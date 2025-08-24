/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_scnd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:00:49 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 18:42:47 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_var(t_full *full, t_pipe **pipe, t_now **get,
		t_general **pipe_blocs)
{
	*pipe = NULL;
	*pipe_blocs = NULL;
	*get = NULL;
	full->get = NULL;
	full->node = NULL;
	full->pipe = NULL;
	full->pipe_blocks = NULL;
	full->new = NULL;
}

void	init_env(t_env **env, char **envp, t_full *full, int *first_run)
{
	*first_run = 1;
	*env = create_env_node();
	if (*first_run)
	{
		get_env(*env, envp);
		full->node = *env;
		*first_run = 0;
	}
}

void	ctrld_free_handler(t_now **get, t_env **env, t_pipe **pipe)
{
	if (*get)
	{
		free_envp(*get);
		*get = NULL;
	}
	if (*env)
	{
		free_env(*env);
		*env = NULL;
	}
	if (*pipe)
	{
		free_pipe(*pipe);
		*pipe = NULL;
	}
}

void	ctrld_free_exit(t_general **pipe_blocs)
{
	int	exit_code;

	if ((*pipe_blocs)->heredoc_fd != -1)
		close((*pipe_blocs)->heredoc_fd);
	if (pipe_blocs)
	{
		exit_code = (*pipe_blocs)->dqm;
		free_pipe_blocks(*pipe_blocs);
		*pipe_blocs = NULL;
	}
	close_all_open_fds();
	exit(exit_code);
}

int	main_line_ctrl_scnd(char *line, t_general **pipe_blocs)
{
	if (line[0] == '\0')
	{
		free_pipe_blocks(*pipe_blocs);
		pipe_blocs = NULL;
		free(line);
		return (1);
	}
	return (0);
}
