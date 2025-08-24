/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:51:59 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 20:22:39 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variables_in_string(t_arg *arg, t_env *env)
{
	char	*new_str;
	int		total_len;

	total_len = expanded_length(arg, env);
	new_str = malloc(sizeof(char) * (total_len + 1));
	if (!new_str)
		return (NULL);
	return (build_expanded_string(arg, env, new_str));
}

int	remove_empty_arg(t_general *node, int i)
{
	int	l;

	l = i;
	free(node->acces_args->args[i]->str);
	free(node->acces_args->args[i]);
	while (node->acces_args->args[l + 1])
	{
		node->acces_args->args[l] = node->acces_args->args[l + 1];
		l++;
	}
	node->acces_args->args[l] = NULL;
	return (1);
}

int	process_single_arg(t_general *node, int i, t_env *env)
{
	char	*new_str;

	new_str = expand_variables_in_string(node->acces_args->args[i], env);
	if (!new_str)
		return (0);
	free(node->acces_args->args[i]->str);
	node->acces_args->args[i]->str = new_str;
	if (!new_str[0])
		return (remove_empty_arg(node, i));
	return (0);
}

void	expand_args_in_node(t_general *node, t_env *env)
{
	int	i;

	i = 0;
	while (node->acces_args->args[i])
	{
		if (skip_expansion(node, i))
		{
			i++;
			continue ;
		}
		if (process_single_arg(node, i, env))
			continue ;
		i++;
	}
}
