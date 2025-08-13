/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_first.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:06:07 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/13 16:33:56 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cont_n(char *str)
{
    int i = 1;
    if (str[0] != '-' || !str[1])
        return 0;
    while (str[i])
    {
        if (str[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}

void	echo_cmd(t_general *tmp, char *str, int i)
{
	int new_line;
	new_line = 1;
	while (tmp->acces_args->args[i] && cont_n(tmp->acces_args->args[i]->str) == 1)
	{
		new_line =0;
		i++;
	}
    	
	while (tmp->acces_args->args[i])
	{
		if (tmp->acces_args->args[i]->str == NULL)
    	{
    	    i++;
    	    continue;
    	}
		if (ft_strcmp(tmp->acces_args->args[i]->str, "-n") == 0)
		{
			if(tmp->acces_args->args[i+1])
				i++;
			else
				break;
		}
		
		if (tmp->acces_args->args[i]&&(ft_strcmp(tmp->acces_args->args[i]->str, ">>") == 0
			|| ft_strcmp(tmp->acces_args->args[i]->str, ">") == 0
			|| ft_strcmp(tmp->acces_args->args[i]->str, "<") == 0
			|| ft_strcmp(tmp->acces_args->args[i]->str, "<<") == 0))
			{
				break ;
			}
			
		str = tmp->acces_args->args[i]->str;
		if (tmp->acces_args->args[i]->flag == 1)
			echo_flag_1(tmp, i);
		else
		{
			if (tmp->acces_args->args[i]->flag == 0
				|| tmp->acces_args->args[i]->flag == 2)
				{
					echo_flag_0_and_2(str, tmp, i);
				}
		}
		i++;
	}
	if (new_line == 1)
		ft_putchar_fd('\n', 1);
	tmp->dqm = 0;
}

void	initalized_echo(t_general *list)
{
	int			i;
	char		*str;
	t_general	*tmp;

	i = 1;
	tmp = list;
	str = NULL;
	if (!tmp->acces_args->args[1])
	{
		ft_putchar_fd('\n', 1);
		return ;
	}
	echo_cmd(tmp, str, i);
}
