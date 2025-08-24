/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 15:29:44 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 17:15:46 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	calculate_var_length(t_arg *arg, t_env *env, int *i)
{
	int		start;
	int		var_len;
	char	*var_name;
	char	*var_value;

	start = ++(*i);
	var_len = 0;
	while (arg->str[*i] && (ft_isalnum(arg->str[*i]) || arg->str[*i] == '_'))
		(*i)++;
	if (*i > start)
	{
		var_name = ft_substr(arg->str, start, *i - start);
		var_value = get_getenv(env, var_name);
		if (var_value)
			var_len = ft_strlen(var_value);
		else
			var_len = 0;
		free(var_name);
	}
	return (var_len);
}

int	expanded_length(t_arg *arg, t_env *env)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (arg->str[i])
	{
		if (arg->str[i] == '$' && arg->str[i + 1] && arg->str[i + 1] != '?'
			&& arg->flag != 1 && arg->str[i + 1] != ' ')
			len += calculate_var_length(arg, env, &i);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

int	is_expandable_variable(t_arg *arg, int pos)
{
	return (arg->str[pos] == '$' && arg->str[pos + 1] && arg->str[pos
			+ 1] != '?' && arg->flag != 1 && arg->str[pos + 1] != ' ');
}

int	expand_variable_at_position(t_arg *arg, t_env *env, char *dest, int *pos)
{
	char	*var_name;
	char	*var_value;
	int		start;
	int		len;

	len = 0;
	start = ++(*pos);
	while (arg->str[*pos] && (ft_isalnum(arg->str[*pos])
			|| arg->str[*pos] == '_'))
		(*pos)++;
	if (*pos > start)
	{
		var_name = ft_substr(arg->str, start, *pos - start);
		if (var_name && arg->flag != 1 && arg->flag != 6)
		{
			var_value = get_getenv(env, var_name);
			if (var_value && var_value[0])
			{
				ft_memcpy(dest, var_value, ft_strlen(var_value));
				len = ft_strlen(var_value);
			}
		}
		free(var_name);
	}
	return (len);
}

char	*build_expanded_string(t_arg *arg, t_env *env, char *new_str)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (arg->str[i])
	{
		if (is_expandable_variable(arg, i))
			k += expand_variable_at_position(arg, env, new_str + k, &i);
		else
			new_str[k++] = arg->str[i++];
	}
	new_str[k] = '\0';
	return (new_str);
}