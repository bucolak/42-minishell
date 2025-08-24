/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_count_malloc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seerel <seerel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:20:50 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 19:28:30 by seerel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	connect_count_malloc(t_general *list)
{
	t_general	*tmp;
	int			i;

	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			if (tmp->acces_args->args[i]->s == 0
				&& tmp->acces_args->args[i + 1])
			{
				if (should_skip_redirection(tmp->acces_args->args, i))
				{
					i++;
					continue ;
				}
				process_adjacent_args(tmp->acces_args->args, i);
				continue ;
			}
			i++;
		}
		tmp = tmp->next;
	}
}
