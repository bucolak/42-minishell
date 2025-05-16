/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/05/16 16:17:00 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_append(t_general *list)
{
	int	i;
	int	fd;

	i = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, ">>") == 0)
		{
			if (list->acces_args->args[i + 1])
			{
				i++;
				fd = open(list->acces_args->args[i]->str,
							O_CREAT | O_WRONLY | O_APPEND,
							0644);
				if (fd < 0)
					perror("open");
				dup2(fd, 1);
				close(fd);
				renew_block2(list);
				break ;
			}
		}
		i++;
	}
}
char	**make_argv(t_pipeafter *acces_args)
{
	int		count;
	char	**argv;

	count = 0;
	while (acces_args->args[count])
		count++;
	argv = malloc(sizeof(char *) * (count + 1));
	for (int i = 0; i < count; i++)
		argv[i] = acces_args->args[i]->str;
	argv[count] = NULL;
	return (argv);
}
void	execute_command(t_general *pipe_blocs, t_now *get)
{
	int		i;
	char	*args;
	char	**paths;
	char	*str;
	char	*end;
	int		command_found;
	char	**argv;

	i = 0;
	command_found = 0;
	args = getenv("PATH");
	paths = ft_split(args, ':');
	while (paths[i])
	{
		str = ft_strjoin(paths[i], "/");
		end = ft_strjoin(str, pipe_blocs->acces_args->args[0]->str);
		if (access(end, X_OK) == 0)
		{
			command_found = 1;
			argv = make_argv(pipe_blocs->acces_args);
			execve(end, argv, get->envp);
			perror("execve\n");
			free(argv);
            free(str);
            free(end);
            // paths'i temizle
            for (int j = 0; paths[j]; j++)
                free(paths[j]);
            free(paths);
            exit(1);
		}
		free(str);
        free(end);
		i++;
	}
	if (!command_found)
	{
		error_msg(2, pipe_blocs->acces_args->args[0]->str, 1);
		exit(pipe_blocs->dqm);
	}
}

void	fill_env(t_env **env, t_now *get)
{
	int		j;
	t_env	*tmp;

	j = 0;
	tmp = *env;
	while (tmp)
	{
		get->envp[j] = malloc(ft_strlen(tmp->key) + ft_strlen(tmp->data) + 2);
		ft_strlcpy(get->envp[j], tmp->key, ft_strlen(tmp->key) + 1);
		ft_strlcpy(get->envp[j] + ft_strlen(tmp->key), tmp->data,
				ft_strlen(tmp->data) + 1);
		if (!get->envp[j])
			return ;
		tmp = tmp->next;
		j++;
	}
	get->envp[j] = NULL;
}

int	is_built_in(char *str)
{
	int	i;

	char *builtin[] = {"echo",
						"pwd",
						"cd",
						"env",
						"export",
						"unset",
						"exit",
						NULL};
	i = 0;
	while (builtin[i])
	{
		if (ft_strcmp(builtin[i], str) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	check_redirection_args2(t_general *pipe_blocs, int *i)
{
	(*i)++;
	if (!pipe_blocs->acces_args->args[*i])
	{
		error_msg(2, NULL, 3);
		exit(1);
	}
	if (access(pipe_blocs->acces_args->args[*i]->str,
				F_OK) != 0)
	{
		error_msg(2, pipe_blocs->acces_args->args[*i]->str, 0);
		exit(1);
	}
}

void	check_redirection_args(t_general *pipe_blocs)
{
	int	i;

	i = 0;
	while (pipe_blocs->acces_args->args[i])
	{
		if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "<<") == 0)
		{
			if (!pipe_blocs->acces_args->args[i + 1])
			{
				error_msg(2, NULL, 3);
				exit(1);
			}
		}
		else if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "<") == 0)
			check_redirection_args2(pipe_blocs, &i);
		else if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, ">") == 0)
		{
			i++;
			if (!pipe_blocs->acces_args->args[i])
			{
				error_msg(2, NULL, 3);
				exit(1);
			}
		}
		i++;
	}
}

void	handle_redirections(t_general *pipe_blocs)
{
	handle_output(pipe_blocs);
	handle_input(pipe_blocs);
	handle_append(pipe_blocs);
	handle_heredoc(pipe_blocs);
}


void	check_cmd_sys_call(t_general *pipe_blocs, t_env **env, t_now *get)
{
	pid_t	pid;

	if (pipe_blocs->next)
	{
		handle_pipe(pipe_blocs, get, env);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		handle_redirections(pipe_blocs);
		if (is_built_in(pipe_blocs->acces_args->args[0]->str) == 1)
		{
			check_cmd_built_in(pipe_blocs, env);
			exit(pipe_blocs->dqm);
		}
		else
		{
			execute_command(pipe_blocs, get);
			exit(pipe_blocs->dqm);
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
		free(get->envp);
        free(get);
	}
}
