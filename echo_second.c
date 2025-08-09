/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:24:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/08 17:32:54 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_flag_0_and_2_second_2(char *str, t_env *env, int j, int *k)
{
	char	*var_name;
	int l;
	char *envv;
	l = 0;
	*k = j+1;
	while (str[*k] && ((str[*k] >= 'A' && str[*k] <= 'Z') || (str[*k] >= 'a'
					&& str[*k] <= 'z') || (str[*k] >= '0' && str[*k] <= '9')
				|| str[*k] == '_'))
			(*k)++;
	var_name = ft_substr(str, j, *k - j);
	if(str[j - 2] && str[j - 2] != '\'')
	{
		envv = get_getenv(env,var_name);
		if (envv)
			ft_putstr_fd(envv, 1);
	}
	else
	{
		while(var_name[l])
		{
			if(var_name[l] != '\'')
				ft_putchar_fd(var_name[l], 1);
			l++;
		}
	}
}

void	echo_flag_0_and_2_second(char *str, t_general *tmp, int *j)
{
	if (str[*j] == '$' && str[*j + 1] == '?')
	{
		ft_putstr_fd(ft_itoa(tmp->dqm), 1);
		tmp->dqm = 0;
			*j += 2;
	}
	else
	{
		if(ft_strcmp(str, "$empty") == 0 || ft_strcmp(str, "$EMPTY") == 0)
		{
			// ft_putchar_fd('\n', 1);
			(*j)+=ft_strlen(str);
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
        echo_flag_0_and_2_second(str, tmp, &j);
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
        ft_putchar_fd(str[j], 1);
        j++;
    }
    if (tmp->acces_args->args[i]->s == 1)
        {
		if(tmp->acces_args->args[i+1])
        	ft_putchar_fd(' ', 1);
	}
}
