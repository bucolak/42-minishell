/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:55:10 by seerel            #+#    #+#             */
/*   Updated: 2025/08/23 15:00:18 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->key)
	{
		free(node->key);
		node->key = NULL;
	}
	if (node->data)
	{
		free(node->data);
		node->data = NULL;
	}
	free(node);
}

void	remove_env_var(t_env **env, char *var_name)
{
	t_env	*curr;
	t_env	*prev;
	size_t	name_len;

	if (!env || !*env || !var_name)
		return ;
	curr = *env;
	prev = NULL;
	name_len = ft_strlen(var_name);
	while (curr)
	{
		if (ft_strcmp(curr->key, var_name) == 0 && (curr->key[name_len] == '='
				|| curr->key[name_len] == '\0'))
		{
			if (!prev)
				*env = curr->next;
			else
				prev->next = curr->next;
			free_env_node(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	unset_cmd(t_general *list, t_env **env)
{
	int	i;

	if (!list || !list->acces_args)
		return ;
	i = 1;
	while (list->acces_args->args[i])
	{
		if (list->acces_args->args[i]->str)
			remove_env_var(env, list->acces_args->args[i]->str);
		i++;
	}
	list->dqm = 0;
}