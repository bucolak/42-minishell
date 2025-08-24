/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handle_first.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:18:56 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 20:58:23 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	control_and_continue_or_exit(int has_command, t_general *pipe_blocs,
		t_full *full)
{
	int	j;

	if (has_command == 0)
	{
		j = 0;
		while (pipe_blocs->acces_args->args[j])
		{
			if (ft_strcmp(pipe_blocs->acces_args->args[j]->str, "<") == 0)
			{
				if (access(pipe_blocs->acces_args->args[j + 1]->str, F_OK) != 0)
					print_message_and_exit(0, pipe_blocs, full, j);
			}
			else if (ft_strcmp(pipe_blocs->acces_args->args[j]->str, ">") == 0)
				print_message_and_exit2(pipe_blocs, full, j);
			else if (ft_strcmp(pipe_blocs->acces_args->args[j]->str, ">>") == 0)
				print_message_and_exit(1, pipe_blocs, full, j);
			j++;
		}
		clean_and_exit(full, pipe_blocs->dqm);
	}
}

static void	has_command_control_redirections(t_general *pipe_blocs,
		int *has_command)
{
	int	i;

	i = 0;
	while (pipe_blocs->acces_args->args[i])
	{
		if (!is_redirection(pipe_blocs->acces_args->args[i]->str)
			&& pipe_blocs->acces_args->args[i]->flag != 5)
		{
			*has_command = 1;
			break ;
		}
		else if (is_redirection(pipe_blocs->acces_args->args[i]->str) && i == 0
			&& (pipe_blocs->acces_args->args[i]->flag == 5
				|| pipe_blocs->acces_args->args[i]->flag == 2))
		{
			*has_command = 0;
			break ;
		}
		i++;
	}
}

void	handle_redirections(t_general *pipe_blocs, t_full *full)
{
	int	i;
	int	is_redirect;
	int	has_command;

	i = 0;
	is_redirect = 0;
	has_command = 0;
	has_command_control_redirections(pipe_blocs, &has_command);
	control_and_continue_or_exit(has_command, pipe_blocs, full);
	while (pipe_blocs->acces_args->args[i])
	{
		control_redirecitons_and_redirect_to_func(pipe_blocs, &is_redirect,
			full, i);
		i++;
	}
	if (is_redirect == 1)
		renew_block2(pipe_blocs);
}
