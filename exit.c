/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:13:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/13 18:07:38 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_cont(t_general *list, int a, t_env *env, t_now *get)
{
	int exit_code;
	if (is_numeric(list->acces_args->args[1]->str))
	{
		a = ft_atoi(list->acces_args->args[1]->str);
		if (a >= 256)
		{
			a -= 256;
			while ((a - 256) >= 0)
				a -= 256;
		}
		list->dqm = a;
		printf("exit\n");
		exit_code = list->dqm;
		free_pipe_blocks(list);
		free_env(env);
		exit(exit_code);
	}
	else
	{
		//ft_putstr_fd("exit\n");
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(list->acces_args->args[1]->str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		list->dqm = 2;
		exit_code = list->dqm;
		free_env(env);
		free_envp(get);
		free_pipe_blocks(list);
		exit(exit_code);
	}
}

void	exit_cmd(t_general *list, t_env *env, t_pipe *pipe, t_now *get)
{
	//int	a;
	int	exit_code;
	//a = 0;
	if (list->acces_args->args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		list->dqm=1;
		return ;
	}
	if (ft_strcmp(list->acces_args->args[0]->str, "exit") == 0)
	{
		if (list->acces_args->args[1])
		{
			exit_cont(list, 0, env, get);
		}
		else
		{
			printf("exit\n");
			list->dqm = 0;
			exit_code = list->dqm;
			free_env(env);
			free_envp(get);
			if(list->next)
				free_pipe(pipe);
			free_pipe_blocks(list);
			free(pipe);
			exit(exit_code);
		}
	}
}
