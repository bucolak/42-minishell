/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function_second.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:50:13 by seerel            #+#    #+#             */
/*   Updated: 2025/08/26 19:37:35 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipe(int count, int **fd)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (pipe(fd[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void	init_pipe(t_pipe *pipe, t_general *list)
{
	pipe->count = 0;
	pipe->tmp = list;
	while (pipe->tmp)
	{
		pipe->count++;
		pipe->tmp = pipe->tmp->next;
	}
	pipe->fd = malloc(sizeof(int *) * (pipe->count - 1));
	pipe->pid = malloc(sizeof(pid_t) * pipe->count);
}

void	signal_handler(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}
