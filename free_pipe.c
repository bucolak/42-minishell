/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:50:36 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 20:24:09 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipe_blocks(t_general *blocks)
{
	t_general	*tmp;
	t_general	*next;

	tmp = blocks;
	while (tmp)
	{
		if (tmp->acces_args)
			free_acces_args(tmp->acces_args);
		if (tmp->limiter)
			free_limiter(tmp->limiter);
		if (tmp->blocs)
			free(tmp->blocs);
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

void	free_pipe(t_pipe *pipe)
{
	int	i;

	i = 0;
	while (i < pipe->count - 1)
	{
		free(pipe->fd[i]);
		i++;
	}
	free(pipe->fd);
	free(pipe->pid);
	free(pipe);
}

void	close_all_open_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void	cleanup(t_full *full)
{
	close_all_open_fds();
	if (full->get)
	{
		free_envp(full->get);
		full->get = NULL;
	}
	if (full->pipe_blocks->next && full->pipe)
	{
		free_pipe(full->pipe);
		full->pipe = NULL;
	}
	if (full->new)
	{
		free_split(full->new);
		full->new = NULL;
	}
	if (full->node)
	{
		free_env(full->node);
		full->node = NULL;
	}
	close_heredoc_fd(full->pipe_blocks);
}
