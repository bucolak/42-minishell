/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_second.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:51:59 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 17:16:26 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_expansion(t_general *node, int i)
{
	return (i > 0 && node->acces_args->args[i - 1]
		&& ft_strcmp(node->acces_args->args[i - 1]->str, "<<") == 0
		&& (node->acces_args->args[i - 1]->flag == 5
			|| node->acces_args->args[i - 1]->flag == 2));
}

void	expand_dollar(t_general *list, t_env *env)
{
	t_general	*tmp;

	tmp = list;
	while (tmp)
	{
		expand_args_in_node(tmp, env);
		tmp = tmp->next;
	}
}