/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_first.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:06:07 by bucolak           #+#    #+#             */
/*   Updated: 2025/06/19 18:35:45 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_flag_4_second(char *str, char *env, t_general *tmp, int *j)
{
	int		k;
	char	*var_name;

	while (str[*j])
	{
		if (str[*j] == '$' && str[*j + 1] != ' ' && str[*j + 1] != '?')
		{
			(*j)++;
			k = *j;
			while (str[k] && str[k] != ' ' && str[k] != '"' && str[k] != '\''
				&& str[k] != '$')
				k++;
			var_name = ft_substr(str, *j, k - *j);
			env = getenv(var_name);
			if (env)
				ft_putstr_fd(env, 1);
			*j = k;
		}
		else if (str[*j] == '$' && str[*j + 1] == '?')
		{
			ft_putstr_fd(ft_itoa(tmp->dqm), 1);
			*j += 2;
		}
		else if (str[*j] != '"' && str[*j] != '\'')
			ft_putchar_fd(str[*j], 1);
		(*j)++;
	}
}

void	echo_flag_4(char *str, char *env, t_general *tmp)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (str[j] == '"')
		{
			j++;
			echo_flag_4_second(str, env, tmp, &j);
			if (!str[j])
				break ;
		}
		if (str[j] != '"' && str[j] != '\'')
		{
			ft_putchar_fd(str[j], 1);
			tmp->dqm = 0;
		}
		j++;
	}
}

void	echo_cmd(t_general *tmp, char *str, char *env, int i)
{

	while (tmp->acces_args->args[i])
	{
		if (ft_strcmp(tmp->acces_args->args[1]->str, "-n") == 0)
			i++;
		if (ft_strcmp(tmp->acces_args->args[i]->str, ">>") == 0
			|| ft_strcmp(tmp->acces_args->args[i]->str, ">") == 0
			|| ft_strcmp(tmp->acces_args->args[i]->str, "<") == 0
			|| ft_strcmp(tmp->acces_args->args[i]->str, "<<") == 0)
			break ;
		str = tmp->acces_args->args[i]->str;
		if (tmp->acces_args->args[i]->flag == 4)
			echo_flag_4(str, env, tmp);
		else if (tmp->acces_args->args[i]->flag == 1)
			echo_flag_1(env, tmp, i);
		else
		{
			if (tmp->acces_args->args[i]->flag == 0
				|| tmp->acces_args->args[i]->flag == 2)
				echo_flag_0_and_2(env, str, tmp, i);
		}
		i++;
	}
	if (tmp->acces_args->args[1] && ft_strcmp(tmp->acces_args->args[1]->str, "-n") != 0) //burda(1)
		ft_putchar_fd('\n', 1);
}

void	initalized_echo(t_general *list)
{
	int			i;
	char		*env;
	char		*str;
	t_general	*tmp;

	i = 1;
	tmp = list;
	env = NULL;
	str = NULL;
	if (!tmp->acces_args->args[1])
	{
		ft_putchar_fd('\n', 1);
	}
	echo_cmd(tmp, str, env, i);
}
