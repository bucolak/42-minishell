/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:45:34 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/23 19:54:16 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_cmd_helper_func(t_env *node, t_env *pre_node, char *s)
{
	while (node)
	{
		if (ft_strcmp(node->key, s) == 0)
		{
			pre_node->next = node->next;
			free(node->key);
			free(node->data);
			free(node);
			return ;
		}
		pre_node = node;
		node = node->next;
	}
}

void	unset_cmd(t_general *list, t_env **env)
{
	int i;
	t_env *node;
	t_env *pre_node;
	char *s;

	i = 1;
	node = *env;
	pre_node = NULL;
	if (ft_strcmp(node->key, list->acces_args->args[i]->str) == 0)
	{
		*env = node->next;
		return ;
	}
	if (list->acces_args->args[i])
	{
		s = list->acces_args->args[i]->str;
		unset_cmd_helper_func(node, pre_node, s);
		list->dqm = 0;
	}
}
