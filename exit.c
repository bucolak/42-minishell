/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:13:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 20:21:30 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_cont(t_full *full, t_general *list)
{
	int	a;

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
		clean_and_exit(full, a);
	}
	else
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(full->pipe_blocks->acces_args->args[1]->str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		list->dqm = 2;
		clean_and_exit(full, list->dqm);
	}
}

void	exit_cmd(t_full *full, t_general *list)
{
	if (list->acces_args->args[1] && list->acces_args->args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		list->dqm = 1;
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
			clean_and_exit(full, list->dqm);
		}
	}
}
