/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/07 17:43:57 by buket            ###   ########.fr       */
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

void signal_handler(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);
}

int has_heredoc(t_general *list)
{
	int i = 0;

	if (!list || !list->acces_args || !list->acces_args->args)
		return 0;
		
	while (list->acces_args->args[i])
	{
		if (list->acces_args->args[i]->str && 
			ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
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

int count_m(t_general *tmp, int i, t_env *env)
{
	int j;
	int c;
	int start;
	char *str;
	char *a;
	
	c = 0;
	j = 0;
	while(tmp->acces_args->args[i]->str[j])
	{
		if(tmp->acces_args->args[i]->str[j] == '$' && 
            tmp->acces_args->args[i]->str[j+1] && 
            tmp->acces_args->args[i]->str[j+1] != '?' && 
            tmp->acces_args->args[i]->flag != 1 &&
			tmp->acces_args->args[i]->str[j+1] != ' ')
			{
				j++;
				start = j;
				while(tmp->acces_args->args[i]->str[j] && tmp->acces_args->args[i]->str[j] != ' ')
					j++;
				a =ft_substr(tmp->acces_args->args[i]->str, start, j-start);
				if(a)
					str = get_getenv(env, a);
				if(str)
					c += ft_strlen(str);
				free(a);
			}
			else
				c++;
		j++;
	}
	return c;
}

void expand_dolar(t_general *list, t_env *env)
{
	t_general *tmp;
	char *new;
	char *str;
	new = NULL;
	str = NULL;
	char *a;
	int i;
	int start;	
	int l;
	int j;
	int k;
	int flag;

	flag = 0;
	tmp = list;
	while(tmp)
	{
		i = 0;
		while(tmp->acces_args->args[i])
		{
			j = 0;
			if((ft_strcmp(tmp->acces_args->args[i]->str, "$empty") == 0 || ft_strcmp(tmp->acces_args->args[i]->str, "$EMPTY") == 0) && tmp->acces_args->args[i]->flag == 2)
			{
				// Argümanı sil
				free(tmp->acces_args->args[i]->str);
				free(tmp->acces_args->args[i]);
				
				// Argümanları kaydır
				l = i;
				while(tmp->acces_args->args[l + 1]) 
				{
					tmp->acces_args->args[l] = tmp->acces_args->args[l+1];
					l++;
				}
				tmp->acces_args->args[l] = NULL;
				
				// i'yi artırma, çünkü kaydırma yaptık
				continue;
			}
			new= malloc(sizeof(char) * (count_m(tmp, i, env) +1));
			k = 0;
			while(tmp->acces_args->args[i]->str[j])
			{
				if(tmp->acces_args->args[i]->str[j] == '$' && 
                   		tmp->acces_args->args[i]->str[j+1] && 
                   		tmp->acces_args->args[i]->str[j+1] != '?' && 
                   		tmp->acces_args->args[i]->flag != 1 &&
						tmp->acces_args->args[i]->str[j+1] != ' ')
				{
					j++;
					start = j;
					while(tmp->acces_args->args[i]->str[j] && tmp->acces_args->args[i]->str[j] != ' ')
						j++;
					a =ft_substr(tmp->acces_args->args[i]->str, start, j-start);
					if (a && tmp->acces_args->args[i]->flag != 1)
					{
					    flag = 1;
					    str = get_getenv(env, a);
					}
					if (str && str[0] != '\0' && flag == 1) 
					{	
					    ft_memcpy(new + k, str, ft_strlen(str));
					    k += ft_strlen(str);
					}
					else
					{
						ft_memcpy(new + k, tmp->acces_args->args[i]->str, ft_strlen(tmp->acces_args->args[i]->str));
						k+= ft_strlen(tmp->acces_args->args[i]->str);
					}
					if(a)
                    	free(a);
				}
				else
				{
					new[k++] = tmp->acces_args->args[i]->str[j];
					j++;
				}
			}
			
			new[k]='\0';
			if(tmp->acces_args->args[i] && tmp->acces_args->args[i]->str)
            {
                free(tmp->acces_args->args[i]->str);
            }
			tmp->acces_args->args[i]->str = new;
			if(tmp->acces_args->args[i] && !tmp->acces_args->args[i]->str[0])
			{
				if(tmp->acces_args->args[i]->str)
					free(tmp->acces_args->args[i]->str);
				if(tmp->acces_args->args[i])
    				free(tmp->acces_args->args[i]);
				l = i;
				while(tmp->acces_args->args[l + 1]) 
    			{
    			    tmp->acces_args->args[l] = tmp->acces_args->args[l+1];
    			    l++;
    			}
    			tmp->acces_args->args[l] = NULL; 
				continue;
			}
			i++;
		}
		tmp = tmp->next;	
	}	
}

char *get_getenv(t_env *env, char *key)
{
	t_env *tmp;

	tmp = env;
	while(tmp)
	{
		if(ft_strncmp(key, tmp->key, ft_strlen(key)) == 0 && tmp->key[ft_strlen(key)] == '=')
			return tmp->data;
		tmp = tmp->next;
	}
	return NULL;
}

void connect_count_malloc(t_general *list)
{
	t_general *tmp;
	tmp = list;
	int i;
	int c;
	int j;
	char *new;
	i = 0;

	while(tmp)
	{
		i = 0;
		while(tmp->acces_args->args[i])
		{
			if(tmp->acces_args->args[i]->s == 0 && tmp->acces_args->args[i+1])
			{
				c = ft_strlen(tmp->acces_args->args[i]->str) + ft_strlen(tmp->acces_args->args[i+1]->str);
				new = malloc(sizeof(char) * (c+1));
				ft_strlcpy(new, tmp->acces_args->args[i]->str, c+1);
                ft_strlcat(new, tmp->acces_args->args[i+1]->str,c+1);
				
				free(tmp->acces_args->args[i+1]->str);
				free(tmp->acces_args->args[i+1]);
				
				free(tmp->acces_args->args[i]->str);
				tmp->acces_args->args[i]->str = new;
				j = i+1;
				while(tmp->acces_args->args[j])
				{
					tmp->acces_args->args[j] = tmp->acces_args->args[j+1];
					j++;
				}
				tmp->acces_args->args[j] = NULL;
				continue;
			}
			i++;
		}
		tmp = tmp->next;
	}	
}
 
int	main(int argc, char *argv[], char **envp)
{
	char		*line;
	t_general	*pipe_blocs;
	t_env		*env;
	t_now		*get;
	t_full full;
	t_pipe	*pipe;
	int last_dqm;
	static int	first_run;
	int exit_code;
	(void)argc;
	(void)argv;
	first_run = 1;
	last_dqm = 0;
	pipe = NULL;
	pipe_blocs = NULL;
	get = NULL;
	env = create_env_node();
	if (first_run)
	{
		get_env(&env, envp);
		full.node = env;
		first_run = 0;
		remove_env_var(&env, "OLDPWD=");
	}
	while (1)
	{
		signal_handler();
		pipe_blocs = create_general_node(last_dqm);
		line = readline("Our_shell% ");
		if (!line)
		{
			if (pipe_blocs)
			{
				exit_code = pipe_blocs->dqm;
				free_pipe_blocks(pipe_blocs);
				pipe_blocs = NULL;
			}
			if (get)
			{
				free_envp(get);
				get = NULL;
			}
			if (env)
			{
				free_env(env);
				env = NULL;
			}
			if (pipe)
			{
				free_pipe(pipe);
				pipe = NULL;
			}
			exit(exit_code);
		}
		if (line[0] == '\0')
		{
			free_pipe_blocks(pipe_blocs);
			pipe_blocs = NULL;
			free(line);
			continue;
		}
		add_history(line);
		pipe_parse(&pipe_blocs, line);
		parse_input(pipe_blocs);
		expand_dolar(pipe_blocs, env);
		connect_count_malloc(pipe_blocs);
		//print_pipes(pipe_blocs);
		full.pipe_blocks = pipe_blocs;
		if(has_heredoc(pipe_blocs) == 1)
		{
			handle_heredoc(pipe_blocs);
			
		}
		get = malloc(sizeof(t_now));
		get->envp = malloc(sizeof(char *) * (ft_lsttsize(env) + 1));
		
		fill_env(&env, get);
		full.get = get;
		if (pipe_blocs->next)
		{
			pipe = malloc(sizeof(t_pipe));
			init_pipe(pipe, pipe_blocs);
			create_pipe(pipe->count, pipe->fd);
			full.pipe = pipe;
			handle_pipe(pipe_blocs, get, &env, pipe,&full);
			
			free_pipe(pipe);
            pipe = NULL;
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
				check_cmd_sys_call(pipe_blocs, &env, get, pipe,&full);
			}
		}
		free_envp(get);
		get = NULL;
		last_dqm = pipe_blocs->dqm;
		if(pipe_blocs->heredoc_fd!=-1)
			close(pipe_blocs->heredoc_fd);
		free_pipe_blocks(pipe_blocs);
		pipe_blocs = NULL;
		free(line);
	}
	free_env(env);
	free_envp(get);
	free_pipe_blocks(pipe_blocs);
	return 0;
}
