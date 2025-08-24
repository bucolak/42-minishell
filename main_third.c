/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_third.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:43:11 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 20:54:05 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_parser(char *line, t_general *pipe_blocs, t_env *env, t_full *full)
{
	add_history(line);
	pipe_parse(&pipe_blocs, line);
	if (parse_input(pipe_blocs) == 1)
		return (1);
	expand_dollar(pipe_blocs, env);
	connect_count_malloc(pipe_blocs);
	remove_null(pipe_blocs);
	control_redireciton(pipe_blocs, env);
	full->pipe_blocks = pipe_blocs;
	return (0);
}

void	fill_get(t_now **get, t_env *env, t_full *full)
{
	*get = malloc(sizeof(t_now));
	(*get)->envp = malloc(sizeof(char *) * (ft_lsttsize(env) + 1));
	fill_env(env, *get);
	full->get = *get;
}

void	apply_pipe(t_general *pipe_blocs, t_pipe **pipe, t_full *full,
		t_env **env)
{
	if (pipe_blocs->next)
	{
		*pipe = malloc(sizeof(t_pipe));
		init_pipe(*pipe, pipe_blocs);
		create_pipe((*pipe)->count, (*pipe)->fd);
		full->pipe = *pipe;
		handle_pipe(pipe_blocs, *env, *pipe, full);
		free_pipe(*pipe);
		*pipe = NULL;
	}
	else if (pipe_blocs->acces_args && pipe_blocs->acces_args->args[0])
	{
		if (!has_redireciton(pipe_blocs)
			&& is_built_in(pipe_blocs->acces_args->args[0]->str))
			check_cmd_built_in(pipe_blocs, env, *pipe, full->get);
		else
			check_cmd_sys_call(pipe_blocs, *env, *pipe, full);
	}
}

void	cleanup_loop_end(t_now **get, t_general **pipe_blocs, char **line,
		int *last_dqm)
{
	if (*get)
	{
		free_envp(*get);
		*get = NULL;
	}
	if (*pipe_blocs)
	{
		*last_dqm = (*pipe_blocs)->dqm;
		free_pipe_blocks(*pipe_blocs);
		*pipe_blocs = NULL;
	}
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
}
