/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:24:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/18 17:26:30 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_flag_0_and_2_second(char *str, t_general *tmp, int *j, int i)
{
	char *new ;
	int start;
	if (str[*j] == '$' && str[*j + 1] && str[*j + 1] == '?')
	{
		new = ft_itoa(tmp->dqm);
		ft_putstr_fd(new, 1);
		free(new);
		tmp->dqm = 0;
			*j += 2;
	}
	else if(str[*j] == '$' && str[*j + 1] && str[*j + 1] != '?')
	{
		(*j)++;
		start = *j;
		while(str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		{
			(*j)++;
		}
		if (start == *j)
    	{
    	    (*j)--;
    	    ft_putchar_fd(str[*j], 1);
    	    (*j)++;
    	}
		new = ft_substr(str, start, (*j)-start);
		if(!new)
		{
			ft_putchar_fd(' ', 1);
		}
		free(new);
	}
	else
	{
		if(str[*j] == '"' && str[*j+1] && str[*j+1] == '"' && tmp->acces_args->args[i+1])
		{
			ft_putchar_fd(' ', 1);
			(*j) += 2;
		}
		else if(str[*j] == '"' && str[*j+1] && str[*j+1] == '"' && !tmp->acces_args->args[i+1])
		{
			(*j) += 2;
		}
		else if(str[*j]!='"')
		{
			ft_putchar_fd(str[*j], 1);
			(*j)++;
		}	
	}
}

void	echo_flag_0_and_2(char *str, t_general *tmp, int i)
{
    int	j;

    j = 0;
    while (str[j])
    {
        echo_flag_0_and_2_second(str, tmp, &j, i);
    }

		if(tmp->acces_args->args[i+1])
		{
        	ft_putchar_fd(' ', 1);
		}
}

void	echo_flag_1(t_general *tmp, int i)
{
    int	j;
    char *str;

    j = 0;
    str = tmp->acces_args->args[i]->str;
    while (str[j])
    {
		if((str[j] == '\'' && str[j+1] && str[j+1] == '\'') && (!str[j-1] || str[j-1] == '\'') && tmp->acces_args->args[i+1])
		{
			ft_putchar_fd(' ', 1);
			j+=2;
		}
		else if((str[j] == '\'' && str[j+1] && str[j+1] == '\'') && (!str[j-1] || str[j-1] == '\'') && !tmp->acces_args->args[i+1])
		{
			j+=2;
		}
		else
		{
			ft_putchar_fd(str[j], 1);
        	j++;
		}
        
    }
    if (tmp->acces_args->args[i]->s == 1)
    {
		if(tmp->acces_args->args[i+1])
        	ft_putchar_fd(' ', 1);
	}
}
