/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:56:22 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/26 20:48:41 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg(int i, char *str, int type, t_general *list)
{
	if (type == 0)
	{
		ft_putstr_fd("bash: ", i);
		ft_putstr_fd(str, i);
		ft_putstr_fd(": No such file or directory\n", i);
		list->dqm = 1;
	}
	else if (type == 1)
	{
		list->dqm = 127;
		ft_putstr_fd("bash: ", i);
		ft_putstr_fd(str, i);
		ft_putstr_fd(": command not found\n", i);
	}
	else if (type == 3)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", i);
		list->dqm = 2;
	}
}
