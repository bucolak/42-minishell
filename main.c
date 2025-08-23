/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 17:24:46 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		signal_ec = 0;

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
					printf(" (flag: %d)", tmp->acces_args->args[i]->flag);
					printf("(s-lag: %d)\n", tmp->acces_args->args[i]->s);
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
		if ((ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<") == 0
				|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<<") == 0
				|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">") == 0
				|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">>") == 0)
			&& (pipe_blocks->acces_args->args[i]->flag == 5))
			return (1);
		i++;
	}
	return (0);
}

void	signal_handler(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}

int	has_heredoc(t_general *list)
{
	int	i;

	i = 0;
	if (!list || !list->acces_args || !list->acces_args->args)
		return (0);
	while (list->acces_args->args[i])
	{
		if (list->acces_args->args[i]->str
			&& ft_strcmp(list->acces_args->args[i]->str, "<<") == 0
			&& (list->acces_args->args[i]->flag == 2
				|| list->acces_args->args[i]->flag == 5))
			return (1);
		i++;
	}
	return (0);
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

int	is_flag_6(t_general *list, t_env *env)
{
	t_env		*tenv;
	int			i;
	t_general	*tmp;

	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			tenv = env;
			if (is_redirection(tmp->acces_args->args[i]->str) == 1
				&& (tmp->acces_args->args[i]->flag == 5
					|| tmp->acces_args->args[i]->flag == 2))
			{
				while (tenv)
				{
					if (ft_strcmp(tenv->data,
							tmp->acces_args->args[i]->str) == 0 && tenv->f == 3)
					{
						return (1);
					}
					tenv = tenv->next;
				}
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}

int	count_m(t_general *tmp, int i, t_env *env)
{
	int		j;
	int		c;
	int		start;
	char	*str;
	char	*a;

	c = 0;
	j = 0;
	if (!tmp->acces_args->args[i]->str || !tmp->acces_args->args[i]->str[0])
		return (0);
	while (tmp->acces_args->args[i]->str[j])
	{
		if (tmp->acces_args->args[i]->str[j] == '$'
			&& tmp->acces_args->args[i]->str[j + 1] != '\0'
			&& tmp->acces_args->args[i]->str[j + 1] != '?'
			&& tmp->acces_args->args[i]->flag != 1
			&& tmp->acces_args->args[i]->str[j + 1] != ' ')
		{
			j++;
			c++;
			start = j;
			while (tmp->acces_args->args[i]->str[j]
				&& (ft_isalnum(tmp->acces_args->args[i]->str[j])
					|| tmp->acces_args->args[i]->str[j] == '_'))
			{
				j++;
			}
			if (j > start)
			{
				a = ft_substr(tmp->acces_args->args[i]->str, start, j - start);
				if (a && tmp->acces_args->args[i]->flag != 1
					&& tmp->acces_args->args[i]->flag != 6)
				{
					str = get_getenv(env, a);
					if (str && str[0])
					{
						c += ft_strlen(str);
					}
					else if (!str)
					{
						if (tmp->acces_args->args[i]->flag == 0)
						{
							c += ft_strlen(tmp->acces_args->args[i]->str);
						}
					}
				}
				free(a);
			}
			continue ;
		}
		else
			c++;
		j++;
	}
	return (c);
}

void	expand_dolar(t_general *list, t_env *env)
{
	t_general	*tmp;
	char		*new;
	char		*str;
	char		*a;
	int			i;
	int			start;
	int			l;
	int			j;
	int			k;
	int			flag;

	new = NULL;
	str = NULL;
	(void)flag;
	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			j = 0;
			if (i > 0 && tmp->acces_args->args[i - 1]
				&& ft_strcmp(tmp->acces_args->args[i - 1]->str, "<<") == 0
				&& (tmp->acces_args->args[i - 1]->flag == 5
					|| tmp->acces_args->args[i - 1]->flag == 2))
			{
				i++;
				continue ;
			}
			new = malloc(sizeof(char) * (count_m(tmp, i, env) + 1));
			if (!new)
				return ;
			k = 0;
			while (tmp->acces_args->args[i]->str[j])
			{
				if (tmp->acces_args->args[i]->str[j] == '$'
					&& tmp->acces_args->args[i]->str[j + 1]
					&& tmp->acces_args->args[i]->str[j + 1] != '?'
					&& tmp->acces_args->args[i]->flag != 1
					&& tmp->acces_args->args[i]->str[j + 1] != ' ')
				{
					j++;
					start = j;
					while (tmp->acces_args->args[i]->str[j]
						&& (ft_isalnum(tmp->acces_args->args[i]->str[j])
							|| tmp->acces_args->args[i]->str[j] == '_'))
					{
						j++;
					}
					if (j > start)
					{
						a = ft_substr(tmp->acces_args->args[i]->str, start, j
								- start);
						if (a && tmp->acces_args->args[i]->flag != 1
							&& tmp->acces_args->args[i]->flag != 6)
						{
							str = get_getenv(env, a);
							if (str && str[0])
							{
								ft_memcpy(new + k, str, ft_strlen(str));
								k += ft_strlen(str);
							}
							else if (!str)
							{
								if (tmp->acces_args->args[i]->flag == 0)
								{
									ft_memcpy(new + k,
										tmp->acces_args->args[i]->str,
										ft_strlen(tmp->acces_args->args[i]->str));
									k
										+= ft_strlen(tmp->acces_args->args[i]->str);
								}
							}
						}
						if (a)
							free(a);
					}
				}
				else
				{
					new[k++] = tmp->acces_args->args[i]->str[j];
					j++;
				}
			}
			new[k] = '\0';
			if (tmp->acces_args->args[i] && tmp->acces_args->args[i]->str)
			{
				free(tmp->acces_args->args[i]->str);
			}
			tmp->acces_args->args[i]->str = new;
			if (tmp->acces_args->args[i] && !tmp->acces_args->args[i]->str[0])
			{
				if (tmp->acces_args->args[i]->str)
					free(tmp->acces_args->args[i]->str);
				if (tmp->acces_args->args[i])
					free(tmp->acces_args->args[i]);
				l = i;
				while (tmp->acces_args->args[l + 1])
				{
					tmp->acces_args->args[l] = tmp->acces_args->args[l + 1];
					l++;
				}
				tmp->acces_args->args[l] = NULL;
				continue ;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

char	*get_getenv(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(key, tmp->key) == 0)
			return (tmp->data);
		tmp = tmp->next;
	}
	return (NULL);
}

void	connect_count_malloc(t_general *list)
{
	t_general	*tmp;
	int			i;
	int			c;
	int			j;
	char		*new;

	tmp = list;
	i = 0;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			if (tmp->acces_args->args[i]->s == 0 && tmp->acces_args->args[i + 1]
				&& ft_strcmp(tmp->acces_args->args[i]->str, "''") != 0
				&& ft_strcmp(tmp->acces_args->args[i]->str, "\"\"") != 0)
			{
				if (is_redirection(tmp->acces_args->args[i]->str) == 1
					&& tmp->acces_args->args[i]->flag != 0
					&& tmp->acces_args->args[i]->flag != 1
					&& is_redirection(tmp->acces_args->args[i + 1]->str) == 0
					&& (tmp->acces_args->args[i + 1]->flag == 0
						|| tmp->acces_args->args[i + 1]->flag == 1))
				{
					i++;
					continue ;
				}
				c = ft_strlen(tmp->acces_args->args[i]->str)
					+ ft_strlen(tmp->acces_args->args[i + 1]->str);
				new = malloc(sizeof(char) * (c + 1));
				ft_strlcpy(new, tmp->acces_args->args[i]->str, c + 1);
				ft_strlcat(new, tmp->acces_args->args[i + 1]->str, c + 1);
				if ((ft_strcmp(new, "<<") == 0 || ft_strcmp(new, ">>") == 0)
					&& ((tmp->acces_args->args[i]->flag != 5
							&& tmp->acces_args->args[i]->flag != 2)
						|| (tmp->acces_args->args[i + 1]->flag != 5
							&& tmp->acces_args->args[i + 1]->flag != 2)))
				{
					tmp->acces_args->args[i]->flag = 0;
				}
				if (tmp->acces_args->args[i + 1]
					&& is_redirection(tmp->acces_args->args[i + 1]->str) == 1
					&& tmp->acces_args->args[i + 1]->flag != 5
					&& tmp->acces_args->args[i + 1]->s == 0)
				{
					tmp->acces_args->args[i]->flag = 6;
				}
				tmp->acces_args->args[i]->s = tmp->acces_args->args[i + 1]->s;
				free(tmp->acces_args->args[i + 1]->str);
				free(tmp->acces_args->args[i + 1]);
				free(tmp->acces_args->args[i]->str);
				tmp->acces_args->args[i]->str = new;
				j = i + 1;
				while (tmp->acces_args->args[j])
				{
					tmp->acces_args->args[j] = tmp->acces_args->args[j + 1];
					j++;
				}
				tmp->acces_args->args[j] = NULL;
				continue ;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

int	count_remove_null(char *str)
{
	int	c;
	int	j;

	c = 0;
	j = 0;
	while (str[j])
	{
		if ((str[j] == '"' && str[j + 1] && str[j + 1] == '"' && ((j > 0
						&& str[j - 1]) || str[j + 2])) || (str[j] == '\''
				&& str[j + 1] && str[j + 1] == '\'' && ((j > 0 && str[j - 1])
					|| str[j + 2])))
		{
			j += 2;
		}
		else
		{
			c++;
			j++;
		}
	}
	return (c + 1);
}

void	remove_null(t_general *list)
{
	t_general	*tmp;
	char		*str;
	int			i;
	int			j;
	int			k;

	tmp = list;
	char *new; //    echo "" "" hi
	i = 0;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			j = 0;
			k = 0;
			str = tmp->acces_args->args[i]->str;
			new = malloc(sizeof(char) * count_remove_null(str));
			if (!new)
				return ;
			while (tmp->acces_args->args[i]->str[j])
			{
				if (((str[j] == '"' && str[j + 1] && str[j + 1] == '"'
							&& ((j > 0 && str[j - 1]) || str[j + 2])))
					|| ((str[j] == '\'' && str[j + 1] && str[j + 1] == '\''
							&& ((j > 0 && str[j - 1]) || str[j + 2]))))
				{
					j += 2;
				}
				else
				{
					new[k++] = str[j++];
				}
			}
			new[k] = '\0';
			if (tmp->acces_args->args[i]->str)
				free(tmp->acces_args->args[i]->str);
			tmp->acces_args->args[i]->str = new;
			i++;
		}
		tmp = tmp->next;
	}
}

void	control_redireciton(t_general *list, t_env *env)
{
	t_general	*tmp;
	char		*str;
	t_env		*tenv;
	int			i;
	int			j;

	str = NULL;
	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			if (tmp->acces_args->args[i]->flag == 6)
			{
				j = 0;
				while (tmp->acces_args->args[i]->str[j])
				{
					if (tmp->acces_args->args[i]->str[j] == '='
						&& tmp->acces_args->args[i]->str[j + 1])
					{
						j++;
						str = tmp->acces_args->args[i]->str + j;
						break ;
					}
					j++;
				}
				tenv = env;
				while (tenv)
				{
					if (ft_strncmp(str, tenv->key, ft_strlen(tenv->key)) == 0)
					{
						tenv->f = 3;
						break ;
					}
					tenv = tenv->next;
				}
			}
			i++;
		}
		tmp = tmp->next;
	}
}

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		signal_ec = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, char *argv[], char **envp)
{
	char		*line;
	t_general	*pipe_blocs;
	t_env		*env;
	t_now		*get;
	t_full		full;
	t_pipe		*pipe;
	int			last_dqm;
	static int	first_run;
	int			exit_code;

	(void)argc;
	(void)argv;
	first_run = 1;
	last_dqm = 0;
	pipe = NULL;
	pipe_blocs = NULL;
	get = NULL;
	full.get = NULL;
	full.node = NULL;
	full.pipe = NULL;
	full.pipe_blocks = NULL;
	full.new = NULL;
	env = create_env_node();
	if (first_run)
	{
		get_env(env, envp);
		full.node = env;
		first_run = 0;
	}
	while (1)
	{
		signal_handler();
		pipe_blocs = create_general_node(last_dqm);
		line = readline("Our_shell% ");
		if (signal_ec == 1)
		{
			last_dqm = 130;
			pipe_blocs->dqm = 130;
			signal_ec = 0;
		}
		if (!line)
		{
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
			close_heredoc_fd(pipe_blocs);
			if (pipe_blocs->heredoc_fd != -1)
				close(pipe_blocs->heredoc_fd);
			if (pipe_blocs)
			{
				exit_code = pipe_blocs->dqm;
				free_pipe_blocks(pipe_blocs);
				pipe_blocs = NULL;
			}
			close_all_open_fds();
			exit(exit_code);
		}
		if (line[0] == '\0')
		{
			free_pipe_blocks(pipe_blocs);
			pipe_blocs = NULL;
			free(line);
			continue ;
		}
		add_history(line);
		pipe_parse(&pipe_blocs, line);
		parse_input(pipe_blocs);
		expand_dolar(pipe_blocs, env);
		connect_count_malloc(pipe_blocs);
		remove_null(pipe_blocs);
		control_redireciton(pipe_blocs, env);
		// print_pipes(pipe_blocs);
		full.pipe_blocks = pipe_blocs;
		get = malloc(sizeof(t_now));
		get->envp = malloc(sizeof(char *) * (ft_lsttsize(env) + 1));
		fill_env(env, get);
		full.get = get;
		if (pipe_blocs->next)
		{
			pipe = malloc(sizeof(t_pipe));
			init_pipe(pipe, pipe_blocs);
			create_pipe(pipe->count, pipe->fd);
			full.pipe = pipe;
			handle_pipe(pipe_blocs, env, pipe, &full);
			free_pipe(pipe);
			pipe = NULL;
		}
		else if (pipe_blocs->acces_args && pipe_blocs->acces_args->args[0])
		{
			if (!has_redireciton(pipe_blocs)
				&& is_built_in(pipe_blocs->acces_args->args[0]->str))
			{
				check_cmd_built_in(pipe_blocs, &env, pipe, get);
			}
			else
			{
				check_cmd_sys_call(pipe_blocs, env, pipe, &full);
			}
		}
		free_envp(get);
		get = NULL;
		last_dqm = pipe_blocs->dqm;
		free_pipe_blocks(pipe_blocs);
		pipe_blocs = NULL;
		free(line);
	}
	free_env(env);
	free_envp(get);
	free_pipe_blocks(pipe_blocs);
	return (0);
}
