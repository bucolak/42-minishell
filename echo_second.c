/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:24:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/06/24 21:01:06 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_flag_0_and_2_second(char *str, char *env, t_general *tmp, int *j)
{
	int		k;
	char	*var_name;
	int l;
	l = 0;


	if (str[*j] == '$' && str[*j + 1] != ' ' && str[*j + 1] != '?' && str[*j
		+ 1] != '"' && str[*j + 1])
	{
		(*j)++;
		k = *j + 1;
		while (str[k] && ((str[k] >= 'A' && str[k] <= 'Z') || (str[k] >= 'a'
					&& str[k] <= 'z') || (str[k] >= '0' && str[k] <= '9')
				|| str[k] == '_'))
			k++;
		var_name = ft_substr(str, *j, k - *j);
		if(str[*j - 2] != '\'')
		{
			env = getenv(var_name);
			if (env)
				ft_putstr_fd(env, 1);
		}
		else
		{
			while(var_name[l])
			{
				if(var_name[l] != '\'')
				{
					ft_putchar_fd(var_name[l], 1);
				}
				l++;
			}
		}
		*j = k;
		free(var_name);
	}
	else if (str[*j] == '$' && str[*j + 1] == '?')
	{
		ft_putstr_fd(ft_itoa(tmp->dqm), 1);
		*j += 2;
	}
	else
	{
		if(str[*j]!='"')
		{
			ft_putchar_fd(str[*j], 1);
		}
		(*j)++;
	}
}

void	echo_flag_0_and_2(char *env, char *str, t_general *tmp, int i)
{
    int	j;

    j = 0;
    while (str[j])
    {
        echo_flag_0_and_2_second(str, env, tmp, &j);
    }
    if (tmp->acces_args->args[i + 1])
        ft_putchar_fd(' ', 1);
}

void	echo_flag_1(char *env, t_general *tmp, int i)
{
    int	j;
	(void)env;
    char *str;

    j = 0;
    str = tmp->acces_args->args[i]->str;
    while (str[j])
    {
        ft_putchar_fd(str[j], 1);
        j++;
    }
    if (tmp->acces_args->args[i + 1])
        ft_putchar_fd(' ', 1);
}
