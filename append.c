/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:43:12 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 12:44:10 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	last_fd_control_append(int last_fd)
{
	if (last_fd != -1)
	{
		dup2(last_fd, 1);
		close(last_fd);
	}
}

static void	handle_append_2(t_full *full, t_general *list, int *fd, int i)
{
	*fd = open(list->acces_args->args[i]->str, O_CREAT | O_WRONLY | O_APPEND,
			0644);
	if (access(list->acces_args->args[i]->str, F_OK) != 0)
	{
		error_msg(2, list->acces_args->args[i]->str, 0, list);
		clean_and_exit(full, list->dqm);
	}
	if (access(list->acces_args->args[i]->str, W_OK) != 0)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(list->acces_args->args[i]->str, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		list->dqm = 1;
		clean_and_exit(full, list->dqm);
	}
	if (*fd < 0)
	{
		error_msg(i, list->acces_args->args[i]->str, 0, list);
		list->dqm = 1;
		clean_and_exit(full, list->dqm);
	}
}

void	handle_append(t_general *list, int i, t_full *full)
{
	int	fd;
	int	last_fd;

	last_fd = -1;
	fd = -1;
	if (ft_strcmp(list->acces_args->args[i]->str, ">>") == 0)
	{
		if (list->acces_args->args[i + 1])
		{
			i++;
			handle_append_2(full, list, &fd, i);
			if (last_fd != -1)
				close(last_fd);
			last_fd = fd;
		}
		else
		{
			error_msg(2, NULL, 3, list);
			clean_and_exit(full, list->dqm);
		}
	}
	last_fd_control_append(last_fd);
}
