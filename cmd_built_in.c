/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:48:09 by buket             #+#    #+#             */
/*   Updated: 2025/06/19 20:27:09 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in_helper_func(t_general *pipe_blocs, t_env **node, int *i)
{
	if (ft_strcmp(pipe_blocs->acces_args->args[*i]->str,
					"pwd") == 0)
		pwd_cmd(&pipe_blocs->acces_args->args[*i]->str, pipe_blocs);
	else if (ft_strcmp(pipe_blocs->acces_args->args[*i]->str,
						"export") == 0)
	{
		if ((pipe_blocs->acces_args->args[*i + 1]
				&& pipe_blocs->acces_args->args[*i + 1]->str)
			&& !pipe_blocs->acces_args->args[*i + 2])
			{
				if(ft_strcmp(pipe_blocs->acces_args->args[*i + 1]->str, "=")==0)
				{
					ft_putstr_fd("bash: export: ", 2);
					ft_putstr_fd("`=': not a valid identifier\n", 2);
					pipe_blocs->dqm=1;
				}
				else
					create_env(pipe_blocs, node);
			}
			
		else if(pipe_blocs->acces_args->args[*i + 2])
			return ;
		else
			print_export_env(node, pipe_blocs);
	}
	else if (ft_strcmp(pipe_blocs->acces_args->args[*i]->str,
						"unset") == 0)
						{
							if(!pipe_blocs->acces_args->args[*i + 1])
								return;
							unset_cmd(pipe_blocs, node);
						}
		
	else if (ft_strcmp(pipe_blocs->acces_args->args[*i]->str,
						"env") == 0)
		print_env(pipe_blocs, node, *i);
	else if (ft_strcmp(pipe_blocs->acces_args->args[*i]->str,
						"exit") == 0)
		exit_cmd(pipe_blocs);
	else if (ft_strcmp(pipe_blocs->acces_args->args[0]->str,
						"$?") == 0)
	{
		ft_putstr_fd(ft_itoa(pipe_blocs->dqm), 2);
		ft_putstr_fd(": command not found\n", 2);
		//pipe_blocs->dqm = 127;
	}
}

void	check_cmd_built_in(t_general *pipe_blocs, t_env **node)
{
	int	i;

	i = 0;
	while (pipe_blocs)
	{
		i = 0;
		while (pipe_blocs->acces_args->args[i])
		{
			if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "cd") == 0)
			{
				cd_cmd(pipe_blocs->acces_args->args, *node, pipe_blocs);
				break ;
			}
			built_in_helper_func(pipe_blocs, node, &i);
			if (ft_strcmp(pipe_blocs->acces_args->args[i]->str,
							"echo") == 0)
				initalized_echo(pipe_blocs);
			i++;
		}
		pipe_blocs = pipe_blocs->next;
	}
}

void	cd_cmd(t_arg **args, t_env *env, t_general *pipe_blocks)
{
	char	*line;

	if (!args[1] || ft_strcmp(args[1]->str, "-") == 0)
	{
		line = getenv("HOME");
		if (line)
			chdir(line);
	}
	else if(args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n",2);
		pipe_blocks->dqm = 1;
	}
	else
	{
		if (args[1]->str[0] == '$' && (args[1]->flag == 0
				|| args[1]->flag == 2))
		{
			args[1]->str++;
			chdir(getenv(args[1]->str));
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
				env->data = getcwd(NULL, 0);
				//BAK!! Fonksiyon otomatik olarak yeterli boyutta bellek allocate eder (malloc ile)
				break ;
			}
			env = env->next;
		}
		chdir(args[1]->str);
	}
}

void	pwd_cmd(char **ar, t_general *list)
{
	char	*line;

	if (ft_strncmp(ar[0], "pwd", 3) == 0)
	{
		line = getcwd(NULL, 0);
		list->dqm = 0;
		if (!line)
		{
			printf("Error\n");
			free(line);
			//free_split(ar);
			exit(1);
		}
		printf("%s\n", line);
	}
}
