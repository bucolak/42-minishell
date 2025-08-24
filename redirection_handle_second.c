/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handle_second.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:20:32 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 20:58:34 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_message_and_exit(int type, t_general *pipe_blocs, t_full *full,
		int j)
{
	int	fd;

	if (type == 0)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(pipe_blocs->acces_args->args[j + 1]->str, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		pipe_blocs->dqm = 1;
		clean_and_exit(full, pipe_blocs->dqm);
	}
	else if (type == 1)
	{
		fd = open(pipe_blocs->acces_args->args[j + 1]->str,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(pipe_blocs->acces_args->args[j + 1]->str, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			pipe_blocs->dqm = 1;
			clean_and_exit(full, pipe_blocs->dqm);
		}
		close(fd);
	}
}

void	print_message_and_exit2(t_general *pipe_blocs, t_full *full, int j)
{
	int	fd;

	fd = open(pipe_blocs->acces_args->args[j + 1]->str,
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(pipe_blocs->acces_args->args[j + 1]->str, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		pipe_blocs->dqm = 1;
		clean_and_exit(full, pipe_blocs->dqm);
	}
	close(fd);
}

void	control_redirecitons_and_redirect_to_func(t_general *pipe_blocs,
		int *is_redirect, t_full *full, int i)
{
	if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "<") == 0
		&& (pipe_blocs->acces_args->args[i]->flag == 2
			|| pipe_blocs->acces_args->args[i]->flag == 5))
	{
		*is_redirect = 1;
		handle_input(pipe_blocs, i, full);
	}
	else if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, ">") == 0
		&& (pipe_blocs->acces_args->args[i]->flag == 2
			|| pipe_blocs->acces_args->args[i]->flag == 5))
	{
		*is_redirect = 1;
		handle_output(pipe_blocs, i, full);
	}
	else if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, ">>") == 0
		&& (pipe_blocs->acces_args->args[i]->flag == 2
			|| pipe_blocs->acces_args->args[i]->flag == 5))
	{
		*is_redirect = 1;
		handle_append(pipe_blocs, i, full);
	}
}
