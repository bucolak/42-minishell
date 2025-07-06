/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/07 00:37:32 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_quotes(const char *line, int pos)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	quote_type = 0;
	while (i < pos)
	{
		if ((line[i] == '"' || line[i] == '\'') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = line[i];
		}
		else if (line[i] == quote_type)
		{
			in_quotes = 0;
			quote_type = 0;
		}
		i++;
	}
	return (in_quotes);
}
void	pipe_parse(t_general **pipe_block, char *line)
{
	int			i;
	int			start;
	t_general	*tmp;
	char		*trimmed;

	i = 0;
	start = 0;
	tmp = *pipe_block;
	while (line[i])
	{
		// Tırnak içinde değilsek ve pipe karakteri bulduysak böl
		if (line[i] == '|' && !is_in_quotes(line, i))
		{
			trimmed = ft_substr(line, start, i - start);
			tmp->blocs = ft_strtrim(trimmed, " ");
			free(trimmed);
			tmp->next = create_general_node(tmp->dqm);
			tmp = tmp->next;
			start = i + 1;
		}
		i++;
	}
	// Son kısmı ekle
	trimmed = ft_substr(line, start, i - start);
	tmp->blocs = ft_strtrim(trimmed, " ");
	free(trimmed);
}

// Tırnak kontrolü için yardımcı fonksiyon

void	print_pipes(t_general *pipe_block)
{
	t_general	*tmp;
	int			pipe_num;
	int			i;

	if (!pipe_block)
		return ;
	tmp = pipe_block;
	pipe_num = 1;
	printf("\n--- Parsed Pipes ---\n");
	while (tmp)
	{
		if (tmp->blocs)
		{
			printf("Pipe %d: [%s]\n", pipe_num, tmp->blocs);
			if (tmp->acces_args && tmp->acces_args->args)
			{
				i = 0;
				printf("  Arguments:\n");
				while (tmp->acces_args->args[i]
					&& tmp->acces_args->args[i]->str)
				{
					printf("    Arg %d: %s", i + 1,
							tmp->acces_args->args[i]->str);
					printf(" (flag: %d)\n", tmp->acces_args->args[i]->flag);
					i++;
				}
			}
			pipe_num++;
		}
		tmp = tmp->next;
	}
	printf("-------------------\n");
}

int	has_redireciton(t_general *pipe_blocks)
{
	int	i;

	i = 0;
	while (pipe_blocks->acces_args->args[i])
	{
		if (ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<") == 0
			|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<<") == 0
			|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">") == 0
			|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">>") == 0)
			return (1);
		i++;
	}
	return (0);
}

void	signal_handler(void)
{
	struct sigaction	sa;
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_handler = handle_signal;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int has_heredoc(t_general *list)
{
	int i = 0;
	while (list->acces_args->args[i])
	{
		if(ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
			return 1;
		i++;
	}
	return 0;
}

int	main(int argc, char *argv[], char **envp)
{
    char		*line;
    t_general	*pipe_blocs;
    t_env		*env;
    t_now		*get;
    static int	first_run;

    pipe_blocs = NULL;
    (void)argc;
    (void)argv;
    first_run = 1;
    env = create_env_node();
    if (first_run)
    {
        get_env(&env, envp);
        first_run = 0;
    }
    while (1)
    {
		pipe_blocs = create_general_node(0);
		if (!pipe_blocs)
            cleanup_and_exit(NULL, env, get, 1);
        line = readline("Our_shell% ");
        if (!line)
		{
			pipe_blocs->dqm = 1;
			cleanup_and_exit(pipe_blocs, env, get, pipe_blocs->dqm);
		}
        if (line[0] == '\0')
        {
            free(line);
			free_pipe_blocks(pipe_blocs);
            continue;
        }
        add_history(line);
        pipe_parse(&pipe_blocs, line);
        signal_handler();
        parse_input(pipe_blocs);
		if(has_heredoc(pipe_blocs) == 1)
        {
            handle_heredoc(pipe_blocs);
        }
        get = malloc(sizeof(t_now));
		if (!get)
            cleanup_and_exit(pipe_blocs, env, NULL, 1);
        get->envp = malloc(sizeof(char *) * (ft_lsttsize(env) + 1));
		if (!get->envp)
        {
            cleanup_and_exit(pipe_blocs, env, get, 1);
        }
        fill_env(&env, get);
		
        if (pipe_blocs->next)
			handle_pipe(pipe_blocs, get, &env);
        else if (pipe_blocs->acces_args && pipe_blocs->acces_args->args[0])
        {
			if ((!has_redireciton(pipe_blocs)
			&& is_built_in(pipe_blocs->acces_args->args[0]->str)))
			{
				check_cmd_built_in(pipe_blocs, &env);
            }
            else
            {
                check_cmd_sys_call(pipe_blocs, &env, get);
            }
        }
		free_envp(get->envp);
		free(get);
		get = NULL;
       	pipe_blocs = create_general_node(pipe_blocs->dqm);
        free(line);
    }
	free_env(env);
	if(get->envp)
		free_envp(get->envp);
	free(get);
	free_pipe_blocks(pipe_blocs);
    return 0;
}