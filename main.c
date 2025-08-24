/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 19:31:54 by bucolak          ###   ########.fr       */
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
	t_state		st;
	char		*line;
	static int	first_run; 

	(void)argc;
	(void)argv;
	st.last_dqm = 0;
	init_var(&st.full, &st.pipe, &st.get, &st.pipe_blocs);
	init_env(&st.env, envp, &st.full, &first_run);
	while (1)
	{
		main_loop_first(&st.last_dqm, &st.pipe_blocs, &line);
		if (!line)
		{
			ctrld_free_handler(&st.get, &st.env, &st.pipe);
			ctrld_free_exit(&st.pipe_blocs);
		}
		if (main_line_ctrl_scnd(line, &st.pipe_blocs) == 1)
			continue ;
		if(apply_parser(line, st.pipe_blocs, st.env, &st.full) == 1)
		{
			free_pipe_blocks(st.pipe_blocs);
			continue;
		}
		fill_get(&st.get, st.env, &st.full);
		apply_pipe(st.pipe_blocs, &st.pipe, &st.full, &st.env);
		cleanup_loop_end(&st.get, &st.pipe_blocs, &line, &st.last_dqm);
	}
	return (0);
}
