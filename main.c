/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/13 18:51:41 by buket            ###   ########.fr       */
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
			// Yeni node oluştur
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

void	init_pipe(t_pipe *pipe, t_general *list)
{
	pipe->count = 0;
	pipe->tmp = list;
	while (pipe->tmp)
	{
		pipe->count++;
		pipe->tmp = pipe->tmp->next;
	}
	pipe->fd = malloc(sizeof(int *) * (pipe->count - 1));
	pipe->pid = malloc(sizeof(pid_t) * pipe->count);
}

void	create_pipe(int count, int **fd)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (pipe(fd[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

int	main(int argc, char *argv[], char **envp)
{
    char		*line;
    t_general	*pipe_blocs;
    t_env		*env;
    t_now		*get;
	t_pipe	*pipe;
	int last_dqm;
    static int	first_run;

    //pipe_blocs = NULL;
    (void)argc;
    (void)argv;
    //pipe_blocs = create_general_node(0);
    first_run = 1;
	last_dqm = 0;
    env = create_env_node();
    if (first_run)
    {
		get_env(&env, envp);
        first_run = 0;
    }
	pipe = malloc(sizeof(t_pipe)); // bunun yerinden çok emin değilim.
    while (1)
    {
		pipe_blocs = create_general_node(last_dqm);
        line = readline("Our_shell% ");
        if (!line)
		{
            exit(1);
		}
        if (line[0] == '\0')
        {
            free(line);
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
        get->envp = malloc(sizeof(char *) * (ft_lsttsize(env) + 1));
        fill_env(&env, get);
		
        if (pipe_blocs->next)
		{
			init_pipe(pipe, pipe_blocs);
			create_pipe(pipe->count, pipe->fd);
			handle_pipe(pipe_blocs, get, &env, pipe);
		}
        else if (pipe_blocs->acces_args && pipe_blocs->acces_args->args[0])
        {
			if ((!has_redireciton(pipe_blocs)
			&& is_built_in(pipe_blocs->acces_args->args[0]->str)))
			{
				check_cmd_built_in(pipe_blocs, &env, pipe, get);
            }
            else
            {
                check_cmd_sys_call(pipe_blocs, &env, get, pipe);
            }
        }
        else
        {
			free_pipe_blocks(pipe_blocs);
        }
		free_envp(get);
		last_dqm = pipe_blocs->dqm;
		free_pipe_blocks(pipe_blocs);
        //pipe_blocs = create_general_node(last_dqm);
        free(line);
    }
	free_env(env);
	free_envp(get);
	free_pipe_blocks(pipe_blocs);
    return 0;
}