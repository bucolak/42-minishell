/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:13:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/16 17:43:35 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_cont(t_general *list, int a)
{
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
		exit(list->dqm);
	}
	else
	{
		printf("exit\n");
		printf("bash: exit: %s: numeric argument required\n",
				list->acces_args->args[1]->str);
		list->dqm = 2;
		exit(list->dqm);
	}
}

void	exit_cmd(t_general *list)
{
	int	a;

	a = 0;
	if (list->acces_args->args[2])
	{
		printf("exit\n");
		printf("bash: exit: too many arguments\n");
		return ;
	}
	if (ft_strcmp(list->acces_args->args[0]->str, "exit") == 0)
	{
		if (list->acces_args->args[1])
		{
			exit_cont(list, a);
		}
		else
		{
			printf("exit\n");
			list->dqm = 0;
			exit(list->dqm);
		}
	}
}
