/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fourth.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:16:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/25 15:00:37 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_heredoc_exit(t_full *full, int fd)
{
	close(fd);
	cleanup(full);
	free_pipe_blocks(full->pipe_blocks);
	exit(0);
}

static void	read_heredoc_input(t_full *full, int fd_write, char *limiter)
{
	char	*line;

	free_heredoc(full);
	close(fd_write - 1);
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("heredoc > ");
		if (!line)
			cleanup_heredoc_exit(full, fd_write);
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			cleanup_heredoc_exit(full, fd_write);
		}
		ft_putstr_fd(line, fd_write);
		ft_putstr_fd("\n", fd_write);
		free(line);
	}
}

int	handle_heredoc_process(t_general *tmp, t_full *full, int *j, int *status)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		read_heredoc_input(full, fd[1], tmp->limiter[*j]);
	close(fd[1]);
	waitpid(pid, status, 0);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(*status) && WEXITSTATUS(*status) == 130)
	{
		tmp->a = 1;
		close(fd[0]);
		remove_heredoc(tmp);
		tmp->heredoc_fd = -1;
		tmp->dqm = 130;
		return (1);
	}
	else if (WIFEXITED(*status))
		return (2);
	tmp->heredoc_fd = fd[0];
	return (0);
}

int	handle_heredoc(t_general *list, t_full *full)
{
	int	result;

	if (!list || !list->acces_args || !list->acces_args->args)
		return (1); // BAK: emin değilim
	fill_limiter(list);
	result = process_heredocs(list, full);
	if (result == 1)
		return (1);
	return (0);
}
