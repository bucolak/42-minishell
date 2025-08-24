/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 17:47:59 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	signal_ec = 0;

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		signal_ec = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

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

void	apply_parser(char *line, t_general *pipe_blocs, t_env *env,
		t_full *full)
{
	add_history(line);
	pipe_parse(&pipe_blocs, line);
	parse_input(pipe_blocs);
	expand_dollar(pipe_blocs, env);
	connect_count_malloc(pipe_blocs);
	remove_null(pipe_blocs);
	control_redireciton(pipe_blocs, env);
	full->pipe_blocks = pipe_blocs;
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

void	end_loop_main(t_now *get, t_general **pipe_blocs, char *line,
		int *last_dqm)
{
	if (get)
		free_envp(get);
	*last_dqm = (*pipe_blocs)->dqm;
	if (*pipe_blocs)
		free_pipe_blocks(*pipe_blocs);
	*pipe_blocs = NULL;
	free(line);
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

void	fill_get(t_now **get, t_env *env, t_full *full)
{
	*get = malloc(sizeof(t_now));
	(*get)->envp = malloc(sizeof(char *) * (ft_lsttsize(env) + 1));
	fill_env(env, *get);
	full->get = *get;
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

void	main_ctrlc_control(int *last_dqm, t_general *pipe_blocs)
{
	if (signal_ec == 1)
	{
		*last_dqm = 130;
		pipe_blocs->dqm = 130;
		signal_ec = 0;
	}
}

void	main_loop_first(int *last_dqm, t_general **pipe_blocs, char **line)
{
	signal_handler();
	*pipe_blocs = create_general_node(*last_dqm);
	*line = readline("Our_shell% ");
	if (signal_ec == 1)
	{
		*last_dqm = 130;
		(*pipe_blocs)->dqm = 130;
		signal_ec = 0;
	}
}

int	main(int argc, char *argv[], char **envp)
{
	char		*line;
	t_general	*pipe_blocs;
	t_env		*env;
	t_now		*get;
	t_full		full;
	t_pipe		*pipe;
	int			last_dqm;
	static int	first_run;

	(void)argc;
	(void)argv;
	last_dqm = 0;
	init_var(&full, &pipe, &get, &pipe_blocs);
	init_env(&env, envp, &full, &first_run);
	while (1)
	{
		main_loop_first(&last_dqm, &pipe_blocs, &line);
		if (!line)
		{
			ctrld_free_handler(&get, &env, &pipe);
			ctrld_free_exit(&pipe_blocs);
		}
		if (main_line_ctrl_scnd(line, &pipe_blocs) == 1)
			continue ;
		apply_parser(line, pipe_blocs, env, &full);
		fill_get(&get, env, &full);
		apply_pipe(pipe_blocs, &pipe, &full, &env);
		cleanup_loop_end(&get, &pipe_blocs, &line, &last_dqm);
	}
	return (0);
}
