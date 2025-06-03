/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:48:09 by buket             #+#    #+#             */
/*   Updated: 2025/06/03 19:14:31 by bucolak          ###   ########.fr       */
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
			create_env(pipe_blocs, node);
		else if(pipe_blocs->acces_args->args[*i + 2])
			return ;
		else
			print_export_env(node, pipe_blocs);
	}
	else if (ft_strcmp(pipe_blocs->acces_args->args[*i]->str,
						"unset") == 0)
		unset_cmd(pipe_blocs, node);
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
				cd_cmd(pipe_blocs->acces_args->args, *node);
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

void	cd_cmd(t_arg **args, t_env *env)
{
	char	*line;

	if (!args[1] || ft_strcmp(args[1]->str, "-") == 0)
	{
		line = getenv("HOME");
		if (line)
			chdir(line);
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
