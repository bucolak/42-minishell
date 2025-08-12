/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:13:46 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/12 15:49:34 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_cont(t_full *full)
{
	int a;
	int exit_code;
	if (is_numeric(full->pipe_blocks->acces_args->args[1]->str))
	{
		a = ft_atoi(full->pipe_blocks->acces_args->args[1]->str);
		if (a >= 256)
		{
			a -= 256;
			while ((a - 256) >= 0)
				a -= 256;
		}
		full->pipe_blocks->dqm = a;
		printf("exit\n");
		exit_code = full->pipe_blocks->dqm;
		cleanup(full);
		free_pipe_blocks(full->pipe_blocks);
		free_env(full->node);
		exit(exit_code);
	}
	else
	{
		//ft_putstr_fd("exit\n");
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(full->pipe_blocks->acces_args->args[1]->str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		full->pipe_blocks->dqm = 2;
		exit_code = full->pipe_blocks->dqm;
		cleanup(full);
		if(full->node)
			free_env(full->node);
		free_pipe_blocks(full->pipe_blocks);
		exit(exit_code);
	}
}

void	exit_cmd(t_full *full)
{
	int	exit_code;
	if (full->pipe_blocks->acces_args->args[1] && full->pipe_blocks->acces_args->args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		full->pipe_blocks->dqm=1;
		return ;
	}
	if (ft_strcmp(full->pipe_blocks->acces_args->args[0]->str, "exit") == 0)
	{
		if (full->pipe_blocks->acces_args->args[1])
		{
			exit_cont(full);
		}
		else
		{
			printf("exit\n");
			full->pipe_blocks->dqm = 0;
			exit_code = full->pipe_blocks->dqm;
			if(full->pipe_blocks->heredoc_fd != -1)
				close(full->pipe_blocks->heredoc_fd);
			if(full->node)
				free_env(full->node);
			if(full->get)
				free_envp(full->get);
			if(full->new)
				free_split(full->new);
			if(full->pipe)
				free_pipe(full->pipe);
			if(full->pipe_blocks)
				free_pipe_blocks(full->pipe_blocks);
			exit(exit_code);
		}
	}
}
