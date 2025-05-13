/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:56:22 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/13 14:58:03 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	message(int i)
{
	int	k;

	if (i == 0)
	{
		printf("%s", "Çift tırnak kapanmadı.");
		k = 0;
	}
	else if (i == 1)
	{
		printf("%s", "Tek tırnak kapanmadı.");
		k = 1;
	}
	return (k);
}

void	error_msg(int i, char *str, int type)
{
	if (type == 0)
	{
		ft_putstr_fd("bash: ", i);
		ft_putstr_fd(str, i);
		ft_putstr_fd(": No such file or directory\n", i);
	}
	else if (type == 1)
	{
		ft_putstr_fd("bash: ", i);
		ft_putstr_fd(str, i);
		ft_putstr_fd(": command not found\n", i);
	}
    else if( type==3)
    {
        ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
									i);
    }
}