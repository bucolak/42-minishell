/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:16:02 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/18 23:11:09 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_block(int count, int i, int **fd)
{
	int	j;

	j = 0;
	close(fd[i - 1][1]);
	close(fd[i][0]); //ortada olan girişini kullanmasına gerek yok.
	dup2(fd[i - 1][0], 0); // bir önceki pipeın okuma ucunu stdine yönlendirdik.
	dup2(fd[i][1], 1); // 2. pipeın yazmasını stdouta yönlendirdik.
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

void	direct_cmd(t_general *tmp, t_now *get, t_env **env, t_pipe *pipe)
{
	handle_redirections(tmp);
	if (is_built_in(tmp->acces_args->args[0]->str) == 1)
	{
		check_cmd_built_in(tmp, env, pipe, get);
		exit(tmp->dqm);
	}
	else
		execute_command(tmp, get,pipe, *env);
}

void	direct_and_close_fd(int count, int **fd, int i, int type)
{
	int	j;

	if (type == 0)
	{
		close(fd[i][0]); //okuma ucunu kapattık. çünkü ilk komut okuma yok.
		dup2(fd[i][1], 1); //pipeın yazma ucunu stdouta yönlendirdik.
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
		if(count != 2)
			close_fd(count, fd, 1, i);
	}
}

void	free_and_wait(int count, pid_t *pid, int **fd)
{
	int	i;

	// i = 0;
	// while (i < count)
	// {
	// 	waitpid(pid[i], NULL, 0);
	// 	i++;
	// }
	i = 0;
	while (i < count - 1)
	{
		free(fd[i]);
		i++;
	}
	free(fd);
	free(pid);
}

void	handle_pipe(t_general *list, t_now *get, t_env **env, t_pipe *pipe)
{
	int status;
	int		i;
	pipe->tmp = list;
	i = 0;
	while (i < pipe->count)
	{
		pipe->pid[i] = fork();
		if (pipe->pid[i] == 0)
		{
			if (i == 0)
				direct_and_close_fd(pipe->count, pipe->fd, i, 0);
			else if (i == pipe->count - 1)
				direct_and_close_fd(pipe->count, pipe->fd, i, 1);
			else
				end_block(pipe->count, i, pipe->fd);
			direct_cmd(pipe->tmp, get, env, pipe);
		}
		i++;
		if (pipe->tmp)
			pipe->tmp = pipe->tmp->next;
	}
	close_fd(pipe->count, pipe->fd, 0, i);
	int last_status = 0;
	for (int j = 0; j < pipe->count; j++)
	{
		waitpid(pipe->pid[j], &status, 0);
		if (j == pipe->count - 1)
			last_status = status;
	}
	if (WIFEXITED(status))
		list->dqm = WEXITSTATUS(last_status);
	//free_and_wait(pipe->count, pipe->pid, pipe->fd);
}
