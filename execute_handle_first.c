/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_handle_first.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/08/24 20:20:23 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_fd(t_general *list)
{
	t_general	*tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->heredoc_fd != -1)
		{
			close(tmp->heredoc_fd);
			tmp->heredoc_fd = -1;
		}
		tmp = tmp->next;
	}
}

static void	signal_handler_child(int a)
{
	(void)a;
	write(1, "\n", 1);
}

static void	wait_child_execute(pid_t pid, t_general *pipe_blocs)
{
	int	status;

	signal(SIGINT, signal_handler_child);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		write(1, "\n", 1);
		pipe_blocs->dqm = 130;
	}
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		pipe_blocs->dqm = WEXITSTATUS(status);
}

static void	handle_command_execute(t_general *pipe_blocs, t_full *full,
		t_env *env, t_pipe *pipe)
{
	if (!pipe_blocs->next || (pipe_blocs->next
			&& has_redireciton(pipe_blocs) == 1))
	{
		if (is_built_in(pipe_blocs->acces_args->args[0]->str) == 1)
		{
			check_cmd_built_in(pipe_blocs, &env, pipe, full->get);
			clean_and_exit(full, pipe_blocs->dqm);
		}
		else
		{
			expand_dollar_qmark_execute(pipe_blocs);
			execute_command(pipe_blocs, full->get, env, full);
			clean_and_exit(full, pipe_blocs->dqm);
		}
	}
}

void	check_cmd_sys_call(t_general *pipe_blocs, t_env *env, t_pipe *pipe,
		t_full *full)
{
	pid_t	pid;

	if (go_to_handle_heredoc(pipe_blocs, full) == 1)
		return ;
	pid = fork();
	if (pipe_blocs->next)
	{
		handle_pipe(pipe_blocs, env, pipe, full);
		return ;
	}
	if (pid == 0)
	{
		if (pipe_blocs->a == 1)
			clean_and_exit(full, 130);
		signal(SIGINT, SIG_DFL);
		if (has_redireciton(pipe_blocs) == 1 && is_flag_6(pipe_blocs, env) == 0)
			handle_redirections(pipe_blocs, full);
		heredoc_fd_dup(pipe_blocs);
		handle_command_execute(pipe_blocs, full, env, pipe);
	}
	wait_child_execute(pid, pipe_blocs);
}
