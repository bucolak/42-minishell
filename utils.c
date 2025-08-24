/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:38:25 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 20:59:53 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_var_len(char *str, int start, t_env *env, int flag)
{
	int		len;
	int		j;
	char	*var_name;
	char	*val;

	len = 0;
	j = start;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	var_name = ft_substr(str, start, j - start);
	if (!var_name)
		return (0);
	if (flag != 1 && flag != 6)
	{
		val = get_getenv(env, var_name);
		if (val && val[0])
			len = ft_strlen(val);
		else if (!val && flag == 0)
			len = ft_strlen(str);
	}
	free(var_name);
	return (len);
}

static int	count_dollar_len(char *str, int *j, t_env *env, int flag)
{
	int	start;
	int	len;

	start = *j;
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		(*j)++;
	len = 0;
	if (*j > start)
		len = get_var_len(str, start, env, flag);
	return (len);
}

int	count_m(t_general *tmp, int i, t_env *env)
{
	int		j;
	int		c;
	char	*str;
	int		flag;

	str = tmp->acces_args->args[i]->str;
	flag = tmp->acces_args->args[i]->flag;
	if (!str || !str[0])
		return (0);
	j = 0;
	c = 0;
	while (str[j])
	{
		if (str[j] == '$' && str[j + 1] != '\0' && str[j + 1] != '?'
			&& flag != 1 && str[j + 1] != ' ')
		{
			j++;
			c++;
			c += count_dollar_len(str, &j, env, flag);
			continue ;
		}
		c++;
		j++;
	}
	return (c);
}

static int	check_env_flag(t_env *env, char *str)
{
	while (env)
	{
		if (ft_strcmp(env->data, str) == 0 && env->f == 3)
			return (1);
		env = env->next;
	}
	return (0);
}

int	is_flag_6(t_general *list, t_env *env)
{
	int			i;
	t_general	*tmp;

	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			if (is_redirection(tmp->acces_args->args[i]->str) == 1
				&& (tmp->acces_args->args[i]->flag == 5
					|| tmp->acces_args->args[i]->flag == 2))
			{
				if (check_env_flag(env, tmp->acces_args->args[i]->str))
					return (1);
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}
