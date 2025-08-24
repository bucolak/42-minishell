/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_second.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:32:17 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 20:52:33 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_heredoc(t_general *list)
{
	int	i;
	int	count;

	if (!list || !list->acces_args || !list->acces_args->args)
		return (0);
	i = 0;
	count = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
			count++;
		i++;
	}
	return (count);
}

void	fill_limiter(t_general *list)
{
	int	i;
	int	j;
	int	heredoc_count;

	if (!list || !list->acces_args || !list->acces_args->args)
		return ;
	heredoc_count = count_heredoc(list);
	list->limiter = malloc(sizeof(char *) * (heredoc_count + 1));
	if (!list->limiter)
		return ;
	i = 0;
	j = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0
			&& list->acces_args->args[i + 1])
		{
			list->limiter[j] = ft_strdup(list->acces_args->args[i + 1]->str);
			j++;
		}
		i++;
	}
	list->limiter[j] = NULL;
}

void	free_heredoc(t_full *full)
{
	static t_full	*temp_full;

	if (full)
	{
		temp_full = full;
	}
	else
	{
		cleanup(temp_full);
		if (temp_full->pipe_blocks)
			free_pipe_blocks(temp_full->pipe_blocks);
		exit(130);
	}
}

void	signal_handler_heredoc(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		free_heredoc(NULL);
		exit(130);
	}
}
