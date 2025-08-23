/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_first.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:06:07 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 17:39:58 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cont_n(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || !str[1])
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	initalized_echo(t_general *list)
{
	int			i;
	t_general	*tmp;

	i = 1;
	tmp = list;
	if (!tmp->acces_args->args[1])
	{
		ft_putchar_fd('\n', 1);
		return ;
	}
	echo_cmd(tmp, i);
}
