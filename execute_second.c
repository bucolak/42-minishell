/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_second.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:29:09 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 15:42:16 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	path_control_execute(char **args, t_general *pipe_blocs, char *cmd,
		t_full *full)
{
	*args = get_getenv(full->node, "PATH");
	if (!args)
	{
		if (ft_strchr(cmd, '/') == NULL && !ft_strstr(get_getenv(full->node,
					"PWD"), "/bin"))
		{
			error_msg(2, cmd, 1, pipe_blocs);
			pipe_blocs->dqm = 127;
			clean_and_exit(full, pipe_blocs->dqm);
		}
	}
}

void	go_to_execve_execute(char **paths, int *command_found, t_full *full,
		t_general *pipe_blocs)
{
	int		i;
	char	*str;
	char	*end;
	char	**argv;

	i = 0;
	while (paths[i])
	{
		str = ft_strjoin(paths[i], "/");
		end = ft_strjoin(str, pipe_blocs->acces_args->args[0]->str);
		if (access(end, X_OK) == 0)
		{
			*command_found = 1;
			argv = make_argv(pipe_blocs->acces_args, full->node);
			heredoc_fd_dup(pipe_blocs);
			execve(end, argv, full->get->envp);
		}
		free(str);
		free(end);
		i++;
	}
}

void	if_command_not_found_execute(int command_found, t_general *pipe_blocs,
		t_full *full, char ***paths)
{
	if (!command_found)
	{
		if (pipe_blocs->acces_args->args[0]->str[0] == '$'
			&& pipe_blocs->acces_args->args[0]->flag != 2)
			error_msg(2, pipe_blocs->acces_args->args[0]->str, 1, pipe_blocs);
		else
			error_msg(2, pipe_blocs->acces_args->args[0]->str, 1, pipe_blocs);
		free_split(*paths);
		clean_and_exit(full, pipe_blocs->dqm);
	}
	free_split(*paths);
}
