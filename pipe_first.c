/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_first.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:16:02 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 17:37:15 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_fd_dup(t_general *list)
{
	if (list->heredoc_fd != -1)
	{
		dup2(list->heredoc_fd, 0);
		close(list->heredoc_fd);
		list->heredoc_fd = -1;
	}
}

static void	direct_cmd(t_general *tmp, t_env *env, t_pipe *pipe, t_full *full)
{
	if (!tmp || !tmp->acces_args || !tmp->acces_args->args
		|| !tmp->acces_args->args[0])
	{
		tmp->dqm = 127;
		clean_and_exit(full, tmp->dqm);
	}
	if (has_redireciton(tmp))
		handle_redirections(tmp, full);
	if (is_built_in(tmp->acces_args->args[0]->str) == 1)
	{
		heredoc_fd_dup(tmp);
		signal(SIGPIPE, SIG_IGN);
		check_cmd_built_in(tmp, &env, pipe, full->get);
		clean_and_exit(full, tmp->dqm);
	}
	else
	{
		execute_command(tmp, full->get, env, full);
		clean_and_exit(full, tmp->dqm);
	}
}

static void	child_process(t_pipe *pipe, t_env *env, t_full *full, int i)
{
	signal(SIGINT, SIG_DFL);
	if (has_heredoc(pipe->tmp) == 1)
		heredoc_fd_dup(pipe->tmp);
	else
	{
		if (i == 0)
			direct_and_close_fd(pipe->count, pipe->fd, i, 0);
		else if (i == pipe->count - 1)
			direct_and_close_fd(pipe->count, pipe->fd, i, 1);
		else
			end_block(pipe->count, i, pipe->fd);
	}
	direct_cmd(pipe->tmp, env, pipe, full);
}

void	handle_pipe(t_general *list, t_env *env, t_pipe *pipe, t_full *full)
{
	int	i;

	pipe->tmp = list;
	i = 0;
	if (go_to_handle_heredoc(list, full) == 1)
		return ;
	while (i < pipe->count)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		pipe->pid[i] = fork();
		if (pipe->pid[i] == 0)
			child_process(pipe, env, full, i);
		i++;
		if (pipe->tmp->next)
			pipe->tmp = pipe->tmp->next;
	}
	close_fd(pipe->count, pipe->fd, 0, i);
	wait_child_pipe(pipe, list);
	close_heredoc_fd(full->pipe_blocks);
}
