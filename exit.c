/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:13:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/14 23:30:08 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_cont(t_full *full, t_general *list)
{
	int a;
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
		cleanup(full);
		free_pipe_blocks(full->pipe_blocks);
		exit(exit_code);
	}
	else
	{
		//ft_putstr_fd("exit\n");
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(full->pipe_blocks->acces_args->args[1]->str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		list->dqm = 2;
		exit_code = list->dqm;
		cleanup(full);
		free_pipe_blocks(full->pipe_blocks);
		exit(exit_code);
	}
}

void	exit_cmd(t_full *full, t_general *list)
{
	int	exit_code;
	if (list->acces_args->args[1] && list->acces_args->args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		list->dqm=1;
		return ;
	}
	if (ft_strcmp(list->acces_args->args[0]->str, "exit") == 0)
	{
		if (list->acces_args->args[1])
		{
			exit_cont(full, list);
		}
		else
		{
			printf("exit\n");
			exit_code = list->dqm;
			cleanup(full);
			if(full->pipe_blocks)
				free_pipe_blocks(full->pipe_blocks);
			exit(exit_code);
		}
	}
}
