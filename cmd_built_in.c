/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:48:09 by buket             #+#    #+#             */
/*   Updated: 2025/07/29 00:29:30 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in_helper_func_2(t_full *full, int i)
{
	if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
					"pwd") == 0)
		pwd_cmd(&full->pipe_blocks->acces_args->args[i]->str, full->pipe_blocks, *full->node);
	else if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
		"unset") == 0)
		{
			if(!full->pipe_blocks->acces_args->args[i + 1])
				return;
			unset_cmd(full->pipe_blocks, full->node);
		}
		
	else if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
						"env") == 0)
		print_env(full->pipe_blocks, full->node, i);
	else if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
						"exit") == 0)
		exit_cmd(full->pipe_blocks, *full->node, full->pipe, full->get);
	else if (ft_strcmp(full->pipe_blocks->acces_args->args[0]->str,
						"$?") == 0)
	{
		ft_putstr_fd(ft_itoa(full->pipe_blocks->dqm), 2);
		ft_putstr_fd(": command not found\n", 2);
		full->pipe_blocks->dqm = 127;
		exit(full->pipe_blocks->dqm);
	}
}

void	built_in_helper_func(t_full *full, int i)
{
	if (ft_strcmp(full->pipe_blocks->acces_args->args[i]->str,
						"export") == 0)
	{
		if ((full->pipe_blocks->acces_args->args[i + 1]
				&& full->pipe_blocks->acces_args->args[i + 1]->str)
			&& !full->pipe_blocks->acces_args->args[i + 2])
			{
				if(ft_strcmp(full->pipe_blocks->acces_args->args[i + 1]->str, "=")==0)
				{
					ft_putstr_fd("bash: export: ", 2);
					ft_putstr_fd("`=': not a valid identifier\n", 2);
					full->pipe_blocks->dqm=1;
				}
				else
					create_env(full->pipe_blocks, full->node);
			}
			
		else if(full->pipe_blocks->acces_args->args[i + 2])
			return ;
		else
			print_export_env(full->node, full->pipe_blocks);
	}
	built_in_helper_func_2(full, i);
}

void	check_cmd_built_in(t_general *pipe_blocs, t_env **node, t_pipe *pipe, t_now *get)
{
	int	i;
	t_full full;
	
	full.pipe_blocks = pipe_blocs;
	full.node = node;
	full.get = get;
	full.pipe = pipe;
	i = 0;
	while (pipe_blocs->acces_args->args[i])
	{
		if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "cd") == 0)
		{
			cd_cmd(pipe_blocs->acces_args->args, *node, pipe_blocs);
			break ;
		}
		built_in_helper_func(&full, i);
		if (ft_strcmp(pipe_blocs->acces_args->args[i]->str,
						"echo") == 0)
		{
							initalized_echo(pipe_blocs, *node);
							break;
		}
		i++;
	}
}

void cd_helper(t_arg **args, char *env_name, t_general *pipe_blocks, t_env *env)
{
	if (args[1]->str[0] == '$' && (args[1]->flag == 0
				|| args[1]->flag == 2))
		{
			env_name = args[1]->str + 1; // bi sorun çıkarsa buraya bi gözat
    		chdir(get_getenv(env ,env_name));
			return ;
		}
		else if(chdir(args[1]->str)==-1)
		{
			perror("cd");
			pipe_blocks->dqm = 1;
		}
		while (env)
		{
			if (ft_strcmp(env->key, "OLDPWD=") == 0)
			{
				free(env->data);
				env->data = getcwd(NULL, 0); //BAK!! Fonksiyon otomatik olarak yeterli boyutta bellek allocate eder (malloc ile)
				break ;
			}
			env = env->next;
		}
		chdir(args[1]->str);
}

void	cd_cmd(t_arg **args, t_env *env, t_general *pipe_blocks)
{
	char	*line;
	char *env_name;
	
	env_name = NULL;
	if (!args[1] || ft_strcmp(args[1]->str, "-") == 0)
	{
		line = get_getenv(env, "HOME");
		if (line)
			chdir(line);
	}
	else if(args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n",2);
		pipe_blocks->dqm = 1;
	}
	else
		cd_helper(args, env_name, pipe_blocks, env);
}

void	pwd_cmd(char **ar, t_general *list, t_env *env)
{
	char	*line;

	if (ft_strncmp(ar[0], "pwd", 3) == 0)
	{
		line = getcwd(NULL, 0);
		list->dqm = 0;
		if (!line)
		{
			printf("Error\n");	line = get_getenv(env, "HOME");
			free(line);
			list->dqm = 1;
			return ;
		}
		printf("%s\n", line);
		free(line);
		list->dqm = 0;
	}
}
