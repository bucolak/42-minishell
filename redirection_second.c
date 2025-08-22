/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seerel <seerel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:51:00 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/22 19:23:55 by seerel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_file_error(char *filename, t_general *list, t_full *full)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	list->dqm = 1;
	cleanup(full);
	free_pipe_blocks(full->pipe_blocks);
	exit(list->dqm);
}

static int	open_output_file(char *filename, t_general *list, t_full *full)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (access(filename, F_OK) != 0)
	{
		error_msg(2, filename, 0, list);
		cleanup(full);
		free_pipe_blocks(full->pipe_blocks);
		exit(list->dqm);
	}
	if (access(filename, W_OK) != 0)
		handle_file_error(filename, list, full);
	if (fd < 0)
	{
		error_msg(2, filename, 0, list);
		list->dqm = 1;
		cleanup(full);
		free_pipe_blocks(full->pipe_blocks);
		exit(list->dqm);
	}
	return (fd);
}

void	handle_output(t_general *list, int i, t_full *full)
{
	int		fd;
	char	*filename;

	if (ft_strcmp(list->acces_args->args[i]->str, ">") == 0)
	{
		if (list->acces_args->args[i + 1])
		{
			filename = list->acces_args->args[i + 1]->str;
			fd = open_output_file(filename, list, full);
			dup2(fd, 1);
			close(fd);
		}
		else
		{
			error_msg(2, NULL, 3, list);
			cleanup(full);
			free_pipe_blocks(full->pipe_blocks);
			exit(list->dqm);
		}
	}
}

static int	open_input_file(char *filename, t_general *list, t_full *full)
{
	int	fd;

	fd = open(filename, O_RDONLY, 0644);
	if (access(filename, F_OK) != 0)
	{
		error_msg(2, filename, 0, list);
		cleanup(full);
		free_pipe_blocks(full->pipe_blocks);
		exit(list->dqm);
	}
	if (access(filename, R_OK) != 0)
		handle_file_error(filename, list, full);
	if (fd < 0)
	{
		error_msg(2, filename, 0, list);
		list->dqm = 1;
		cleanup(full);
		free_pipe_blocks(full->pipe_blocks);
		exit(list->dqm);
	}
	return (fd);
}

void	handle_input(t_general *list, int i, t_full *full)
{
	int		fd;
	char	*filename;

	if (ft_strcmp(list->acces_args->args[i]->str, "<") == 0)
	{
		if (list->acces_args->args[i + 1])
		{
			filename = list->acces_args->args[i + 1]->str;
			fd = open_input_file(filename, list, full);
			dup2(fd, 0);
			close(fd);
		}
		else
		{
			error_msg(2, NULL, 3, list);
			cleanup(full);
			free_pipe_blocks(full->pipe_blocks);
			exit(list->dqm);
		}
	}
}
