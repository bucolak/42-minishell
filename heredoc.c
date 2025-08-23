/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:26:51 by seerel            #+#    #+#             */
/*   Updated: 2025/08/23 19:14:46 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	go_to_handle_heredoc(t_general *list, t_full *full)
{
	t_general	*tmp;

	tmp = list;
	while (tmp)
	{
		if (has_heredoc(tmp) == 1)
			handle_heredoc(tmp, full);
		if (tmp->a == 1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static t_arg	**build_heredoc_clean_args(t_arg **args)
{
	t_arg	**new_arg;
	int		i;
	int		j;
	int		count;

	count = 0;
	while (args[count])
		count++;
	new_arg = malloc(sizeof(t_arg *) * (count + 1));
	if (!new_arg)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i]->str, "<<") == 0)
		{
			i += 2;
			continue ;
		}
		new_arg[j++] = args[i++];
	}
	new_arg[j] = NULL;
	return (new_arg);
}

static void	free_heredoc_args(t_arg **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i]->str, "<<") == 0)
		{
			free(args[i]->str);
			free(args[i]);
			i++;
			if (args[i])
			{
				free(args[i]->str);
				free(args[i]);
				i++;
			}
			continue ;
		}
		i++;
	}
	free(args);
}

void	remove_heredoc(t_general *list)
{
	t_arg	**old_args;
	t_arg	**new_args;

	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	old_args = list->acces_args->args;
	new_args = build_heredoc_clean_args(old_args);
	if (!new_args)
		return ;
	list->acces_args->args = new_args;
	free_heredoc_args(old_args);
}