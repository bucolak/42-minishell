/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 21:19:43 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/22 21:28:39 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_block(int count, int i, int **fd)
{
	int	j;

	j = 0;
	close(fd[i - 1][1]);
	close(fd[i][0]);
	dup2(fd[i - 1][0], 0);
	dup2(fd[i][1], 1);
	close(fd[i - 1][0]);
	close(fd[i][1]);
	while (j < count - 1)
	{
		if (j != i - 1 && j != i)
		{
			close(fd[j][0]);
			close(fd[j][1]);
		}
		j++;
	}
}

void	close_fd(int count, int **fd, int type, int i)
{
	int	a;

	a = 0;
	if (type == 0)
	{
		while (a < count - 1)
		{
			close(fd[a][0]);
			close(fd[a][1]);
			a++;
		}
	}
	else if (type == 1)
	{
		a = 0;
		while (a < count - 2)
		{
			if (a != i - 1)
			{
				close(fd[a][0]);
				close(fd[a][1]);
			}
			a++;
		}
	}
}

void	clean_and_exit(t_full *full, int ec)
{
	int	exit_code;

	exit_code = ec;
	cleanup(full);
	if (full->pipe_blocks)
		free_pipe_blocks(full->pipe_blocks);
	exit(exit_code);
}

void	direct_and_close_fd(int count, int **fd, int i, int type)
{
	int	j;

	if (type == 0)
	{
		close(fd[i][0]);
		dup2(fd[i][1], 1);
		close(fd[i][1]);
		j = 1;
		while (j < count - 1)
		{
			close(fd[j][0]);
			close(fd[j][1]);
			j++;
		}
	}
	else if (type == 1)
	{
		close(fd[i - 1][1]);
		dup2(fd[i - 1][0], 0);
		close(fd[i - 1][0]);
		if (count != 2)
			close_fd(count, fd, 1, i);
	}
}

void	wait_child(t_pipe *pipe, t_general *list)
{
	int	j;
	int	last_status;
	int	status;

	j = 0;
	last_status = 0;
	while (j < pipe->count)
	{
		waitpid(pipe->pid[j], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			pipe->tmp->dqm = 130;
			break ;
		}
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, SIG_IGN);
		if (j == pipe->count - 1)
			last_status = status;
		if (WIFEXITED(status))
			list->dqm = WEXITSTATUS(last_status);
		j++;
	}
}
