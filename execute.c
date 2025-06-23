/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/06/23 10:19:14 by buket            ###   ########.fr       */
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
	char *cmd;
	char *env;
	
	cmd = pipe_blocs->acces_args->args[0]->str;
	if(cmd[0] == '$')
	{
		env = getenv(cmd+1);
		if(env)
			cmd = env;
		else
		{
			if(pipe_blocs->acces_args->args[1])
			{
				cmd = pipe_blocs->acces_args->args[1]->str;
				i = 1;
				while(pipe_blocs->acces_args->args[i])
				{
					pipe_blocs->acces_args->args[i-1] = pipe_blocs->acces_args->args[i];
					i++;
				}	
				pipe_blocs->acces_args->args[i-1] = NULL;
			}
			else
			{
				exit(0);
			}
		}
	}
	
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
			if(pipe_blocs->heredoc_fd!=-1)
			{
				dup2(pipe_blocs->heredoc_fd, 0);
				close(pipe_blocs->heredoc_fd);
			}
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
	if (!command_found && pipe_blocs->acces_args->args[0]->str[0] != '$')
	{
		//printf("burda\n");
		error_msg(2, pipe_blocs->acces_args->args[0]->str, 1, pipe_blocs);
		exit(pipe_blocs->dqm);
	}
	else if(pipe_blocs->acces_args->args[0]->str[0] == '$')
	{
		pipe_blocs->acces_args->args[0]->str++;
		if(getenv(pipe_blocs->acces_args->args[0]->str))
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(getenv(pipe_blocs->acces_args->args[0]->str), 2);
			ft_putstr_fd(": Is a directory\n", 2);
		}
		if(getenv(pipe_blocs->acces_args->args[0]->str))
			pipe_blocs->dqm = 126;
		else
			pipe_blocs->dqm = 0;
		
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
						"$?",
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
		error_msg(2, NULL, 3, pipe_blocs);
		exit(1);
	}
	if (access(pipe_blocs->acces_args->args[*i]->str,
				F_OK) != 0)
	{
		error_msg(2, pipe_blocs->acces_args->args[*i]->str, 0, pipe_blocs);
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
				error_msg(2, NULL, 3, pipe_blocs);
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
				error_msg(2, NULL, 3, pipe_blocs);
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
	//handle_heredoc(pipe_blocs);
}

void	check_cmd_sys_call(t_general *pipe_blocs, t_env **env, t_now *get)
{
	//printf("burxda\n");
	int		status;
	pid_t	pid;
	status = 0;
	pid = fork();
	if (pipe_blocs->next)
	{
		handle_pipe(pipe_blocs, get, env);
		return ;
	}
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
			// if (pipe_blocs->heredoc_fd != -1)
			// {
			// 	printf("hey\n");
			// 	dup2(pipe_blocs->heredoc_fd, 0);
			// 	close(pipe_blocs->heredoc_fd);
			// }
			execute_command(pipe_blocs, get);
			exit(pipe_blocs->dqm);
		}
	}
	else
	{
		// if (pipe_blocs->heredoc_fd != -1)
		// {
		// 	close(pipe_blocs->heredoc_fd);
		// 	pipe_blocs->heredoc_fd = -1;
		// }
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			pipe_blocs->dqm = WEXITSTATUS(status);
		free(get->envp);
		free(get);
	}
}

// INFO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//Child process bittiğinde, çıkış statüsü (exit code ve sinyal bilgisi) status değişkenine yazılır.
//Yani: Bu satırdan sonra, child’ın nasıl bittiği bilgisi status’tedir.      ---------->  waitpid(pid,&status, 0);

//WIFEXITED(status) makrosu,child process’in normal şekilde (ör. exit(5);) sonlanıp sonlanmadığını kontrol eder.
//Eğer true ise,child normal şekilde (örneğin bir exit koduyla) bitti demektir.  --------->   if(WIFEXITED(status))

//WEXITSTATUS(status) ile,child process’in exit() veya return ile döndürdüğü exit kodunu alırsın.
//Bunu pipe_blocs->dqm'ye atayarak,shell’in içinde $? ile gösterilecek olan exit status’u güncellersin. --->  pipe_blocs->dqm = WEXITSTATUS(status);
