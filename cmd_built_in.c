/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:48:09 by buket             #+#    #+#             */
/*   Updated: 2025/08/24 20:04:35 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	built_in_helper_func_2(t_full *full, int i, t_env **env,
		t_general *list)
{
	if (ft_strcmp(list->acces_args->args[i]->str, "pwd") == 0)
		pwd_cmd(&list->acces_args->args[i]->str, list, *env);
	else if (ft_strcmp(list->acces_args->args[i]->str, "unset") == 0)
	{
		if (!list->acces_args->args[i + 1])
			return ;
		unset_cmd(list, env);
		full->node = *env;
	}
	else if (ft_strcmp(list->acces_args->args[i]->str, "env") == 0)
		print_env(list, *env, i);
	else if (ft_strcmp(list->acces_args->args[i]->str, "exit") == 0)
		exit_cmd(full, list);
}

static void	free_new_builtin_helper(t_full *full)
{
	if (full->new)
	{
		free_split(full->new);
		full->new = NULL;
	}
}

static void	built_in_helper_func(t_full *full, int i, t_env **envv,
		t_general *list)
{
	t_general	*tmp;
	int			c;

	tmp = list;
	c = 0;
	if (ft_strcmp(tmp->acces_args->args[i]->str, "export") == 0)
	{
		if ((list->acces_args->args[i + 1] && tmp->acces_args->args[i
					+ 1]->str))
		{
			if (ctrl_is_valid_identifier_builtin_helper(tmp, i) == 1)
				return ;
			export_cntrl_builtin_helper(tmp, i, &c, full);
			export_cntrl_builtin_helper_scnd(full, tmp);
			create_env(tmp, envv, full);
			put_flag_builtin_helper(full);
		}
		else
			print_export_env(*envv, full->pipe_blocks);
	}
	built_in_helper_func_2(full, i, envv, list);
	free_new_builtin_helper(full);
}

static int	control_cmd_builtin(int i, t_general *pipe_blocs, t_env **node)
{
	if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "cd") == 0)
	{
		cd_cmd(pipe_blocs->acces_args->args, *node, pipe_blocs);
		return (1);
	}
	else if (ft_strcmp(pipe_blocs->acces_args->args[0]->str, "$?") == 0)
	{
		ft_putstr_fd(": command not found\n", 2);
		pipe_blocs->dqm = 127;
		return (1);
	}
	return (0);
}

void	check_cmd_built_in(t_general *pipe_blocs, t_env **node, t_pipe *pipe,
		t_now *get)
{
	int		i;
	t_full	full;

	full.pipe_blocks = pipe_blocs;
	full.node = *node;
	full.get = get;
	full.pipe = pipe;
	full.new = NULL;
	i = 0;
	while (pipe_blocs->acces_args->args[i])
	{
		if (control_cmd_builtin(i, pipe_blocs, node) == 1)
			break ;
		built_in_helper_func(&full, i, node, pipe_blocs);
		if (ft_strcmp(pipe_blocs->acces_args->args[i]->str, "echo") == 0)
		{
			initalized_echo(pipe_blocs);
			break ;
		}
		i++;
	}
}
