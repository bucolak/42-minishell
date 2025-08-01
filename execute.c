/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/08/02 01:17:55 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_append_2(char *last_input, t_general *list, int *fd, int i)
{
	last_input = list->acces_args->args[i]->str;
	*fd = open(last_input, O_CREAT | O_WRONLY | O_APPEND,
			0644);
	if(access(last_input, F_OK) != 0)
	{
		error_msg(2, last_input, 0, list);
		free_pipe_blocks(list);
		exit(list->dqm);
	}
	if (access(last_input, W_OK) != 0)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(last_input, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free_pipe_blocks(list);
		list->dqm = 1;
		exit(list->dqm);
	}
	if (*fd < 0)
	{
		error_msg(i, list->acces_args->args[i]->str, 0, list);
		list->dqm = 1;
		free_pipe_blocks(list);
		exit(list->dqm);
	}
}

void	handle_append(t_general *list, int i)
{
	int	fd;
	char *last_input;
	int last_fd;

	last_fd = -1;
	last_input = NULL;
	fd = -1;
	if (ft_strcmp(list->acces_args->args[i]->str, ">>") == 0)
	{
		if (list->acces_args->args[i + 1])
		{
			i++;
			handle_append_2(last_input,list,&fd,i);
			if(last_fd !=-1)
				close(last_fd);
			last_fd = fd;
		}
		else
		{
			error_msg(2, NULL, 3, list);
			free_pipe_blocks(list);
			exit(list->dqm) ;
		}
	}
	if(last_fd!=-1)
	{
		dup2(last_fd, 1);
		close(last_fd);
	}
}

char * extend_env(char *str, t_env *env)
{
	char *new;
	new =get_getenv(env,str);
	return new;
}
char	**make_argv(t_pipeafter *acces_args, t_env *env)
{
	int		count;
	char	**argv;

	count = 0;
	while (acces_args->args[count])
		count++;
	argv = malloc(sizeof(char *) * (count + 1));
	for (int i = 0; i < count; i++)
	{
		if((acces_args->args[i]->flag == 0 || acces_args->args[i]->flag == 2) && acces_args->args[i]->str[0] == '$')
		{
			argv[i]=extend_env(acces_args->args[i]->str+1, env);
		}
		else
			argv[i] = acces_args->args[i]->str;
	}
	argv[count] = NULL;
	return (argv);
}

void	execute_command(t_general *pipe_blocs, t_now *get, t_pipe *pipe, t_env *envv)
{
	int		i;
	char	*args;
	char	**paths;
	char	*str;
	char *new;
	char	*end;
	int		command_found;
	char	**argv;
	char *cmd;
	char *env;
	struct stat sb;
	int exit_code;
	cmd = pipe_blocs->acces_args->args[0]->str;
	if (ft_strchr(cmd, '/')) 
	{
		if(stat(cmd, &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			pipe_blocs->dqm = 126;
			exit(pipe_blocs->dqm);
		}
		else if (access(cmd, F_OK) != 0) 
		{
			error_msg(2, cmd, 0, pipe_blocs);
			pipe_blocs->dqm = 127;
			exit(pipe_blocs->dqm);
		} 
		else if (access(cmd, X_OK) != 0) 
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			pipe_blocs->dqm = 126;
			exit(pipe_blocs->dqm);
		}
		if (pipe_blocs->acces_args->args[1] && ft_strcmp(pipe_blocs->acces_args->args[1]->str, "$?") == 0 && pipe_blocs->acces_args->args[1]->flag != 1)
		{
			free(pipe_blocs->acces_args->args[1]->str);
			pipe_blocs->acces_args->args[1]->str = ft_itoa(pipe_blocs->dqm);
		}
 		argv = make_argv(pipe_blocs->acces_args, envv);
		execve(cmd, argv, get->envp);
		pipe_blocs->dqm = 0;
		exit(pipe_blocs->dqm);
	}
	if(cmd[0] == '$')
	{
			env = get_getenv(envv,cmd+1);
		if(ft_strcmp(cmd, "$") == 0)
		{
			ft_putstr_fd("$: command not found\n", 2);
			pipe_blocs->dqm = 127;
			exit(pipe_blocs->dqm);
		}
		else if(env)
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
				pipe_blocs->dqm = 0;
				exit(pipe_blocs->dqm);
			}
		}
	}
	i = 0;
	command_found = 0;
	args = get_getenv(envv,"PATH");
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
			argv = make_argv(pipe_blocs->acces_args, envv);
			execve(end, argv, get->envp);
			perror("execve\n");
			free(argv);
			free(str);
			free(end);
			// paths'i temizle
			for (int j = 0; paths[j]; j++)
				free(paths[j]);
			free(paths);
			free_pipe(pipe);
			pipe_blocs->dqm = 0;
			exit_code = pipe_blocs->dqm;
			free_pipe_blocks(pipe_blocs);
			//exit(exit_code);
		}
			free(str);
			free(end);
		
		i++;
	}
	if (!command_found && pipe_blocs->acces_args->args[0]->str[0] != '$')
	{
		error_msg(2, pipe_blocs->acces_args->args[0]->str, 1, pipe_blocs);
		free_envp(get);
		for (int j = 0; paths[j]; j++)
			free(paths[j]);
		free(paths);
		free_env(envv);
		if(pipe_blocs->next)
			free_pipe(pipe);
		exit_code = pipe_blocs->dqm;
		free_pipe_blocks(pipe_blocs);
		exit(exit_code);
	}
	else if(pipe_blocs->acces_args->args[0]->str[0] == '$')
	{
		new = pipe_blocs->acces_args->args[0]->str+1;
		if(get_getenv(envv, new))
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(get_getenv(envv, new), 2);
			ft_putstr_fd(": Is a directory\n", 2);
		}
		if(get_getenv(envv, new))
			pipe_blocs->dqm = 126;
		else
			pipe_blocs->dqm = 0;
	}
	for (int j = 0; paths[j]; j++)
		free(paths[j]);
	free(paths);
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
	int i = 0;
	int is_redirect = 0;
	while(pipe_blocs->acces_args->args[i])
	{
		if(ft_strcmp(pipe_blocs->acces_args->args[i]->str, "<") == 0)
		{
			is_redirect = 1;
			handle_input(pipe_blocs, i);
		}
		else if(ft_strcmp(pipe_blocs->acces_args->args[i]->str, ">") == 0)
		{
			is_redirect = 1;
			handle_output(pipe_blocs, i);
		}
		else if(ft_strcmp(pipe_blocs->acces_args->args[i]->str, ">>") == 0)
		{
			is_redirect = 1;
			handle_append(pipe_blocs, i);
		}
		i++;
	}
	if(is_redirect == 1)
		renew_block2(pipe_blocs);
}

int count_malloc(t_general *list, int j)
{
	int c;
	int i;
	char *str;
	char *itoa;
	i = 0;
	c = 0;
	if(list->acces_args->args[j])
	{
		str = list->acces_args->args[j]->str;
		while(str[i])
		{
			if(str[i] == '$' && str[i] && str[i] == '?')
			{
				itoa = ft_itoa(list->dqm);
				c+=ft_strlen(itoa);
				free(itoa);
				i++;
			}
			else
				c++;
			i++;
		}
	}
	return c;
}

void expand_dolar_qmark(t_general *list)
{
	char *str;
	int i;
	int j;
	int k;
	char *new;
	j = 0;
	while(list->acces_args->args[j])
	{
		i = 0;
		k = 0;
		new = malloc(sizeof(char *) * (count_malloc(list, j) + 1));
		str = list->acces_args->args[j]->str;
		while(str[i])
		{
			if(str[i] == '$' && str[i+1] && str[i+1] == '?')
			{
				ft_memcpy(new+k, ft_itoa(list->dqm), ft_strlen(ft_itoa(list->dqm)));
				i+=2;
				k+=ft_strlen(ft_itoa(list->dqm));
			}
			else
				new[k++] = str[i++];
		}
		new[k] = '\0';
		free(list->acces_args->args[j]->str);
		list->acces_args->args[j]->str = ft_strdup(new);
		free(new);
		j++;
	}
}

void	check_cmd_sys_call(t_general *pipe_blocs, t_env **env, t_now *get, t_pipe *pipe)
{
	int		status;
	int e;
	pid_t	pid;
	status = 0;
	pid = fork();
	if (pipe_blocs->next)
	{
		handle_pipe(pipe_blocs, get, env, pipe);
		return ;
	}
	if (pid == 0)
	{
		handle_redirections(pipe_blocs);
		if (is_built_in(pipe_blocs->acces_args->args[0]->str) == 1)
		{
			check_cmd_built_in(pipe_blocs, env, pipe, get);
			exit(pipe_blocs->dqm);
		}
		else
		{
			expand_dolar_qmark(pipe_blocs);
			execute_command(pipe_blocs, get, pipe, *env);
			free_envp(get);
			free_env(*env);
			e = pipe_blocs->dqm;
			free_pipe_blocks(pipe_blocs);
			exit(e); //buradan önce pipe_blocks'un free edilmesi gerektiğini düşünüyorum ama error'e sebep oluyo
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			pipe_blocs->dqm = WEXITSTATUS(status);
	}
}

// INFO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//Child process bittiğinde, çıkış statüsü (exit code ve sinyal bilgisi) status değişkenine yazılır.
//Yani: Bu satırdan sonra, child’ın nasıl bittiği bilgisi status’tedir.      ---------->  waitpid(pid,&status, 0);

//WIFEXITED(status) makrosu,child process’in normal şekilde (ör. exit(5);) sonlanıp sonlanmadığını kontrol eder.
//Eğer true ise,child normal şekilde (örneğin bir exit koduyla) bitti demektir.  --------->   if(WIFEXITED(status))

//WEXITSTATUS(status) ile,child process’in exit() veya return ile döndürdüğü exit kodunu alırsın.
//Bunu pipe_blocs->dqm'ye atayarak,shell’in içinde $? ile gösterilecek olan exit status’u güncellersin. --->  pipe_blocs->dqm = WEXITSTATUS(status);
