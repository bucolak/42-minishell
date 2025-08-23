/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_first.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:28:21 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 15:44:08 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extend_env(char *str, t_env *env)
{
	char	*new;

	new = get_getenv(env, str);
	return (new);
}

char	**make_argv(t_pipeafter *acces_args, t_env *env)
{
	int		count;
	char	**argv;

	count = 0;
	while (acces_args->args[count])
		count++;
	argv = malloc(sizeof(char *) * (count + 1));
	for (int i = 0; i < count; i++)
	{
		if ((acces_args->args[i]->flag == 0 || acces_args->args[i]->flag == 2)
			&& acces_args->args[i]->str[0] == '$')
		{
			argv[i] = extend_env(acces_args->args[i]->str + 1, env);
		}
		else
			argv[i] = acces_args->args[i]->str;
	}
	argv[count] = NULL;
	return (argv);
}

static void	if_have_dollar_execute(char **cmd, t_general *pipe_blocs, t_env *envv,
		t_full *full)
{
	char	*env;

	if (*cmd[0] == '$' && pipe_blocs->acces_args->args[0]->flag != 1)
	{
		env = get_getenv(envv, *cmd + 1);
		if (ft_strcmp(*cmd, "$") == 0)
		{
			ft_putstr_fd("$: command not found\n", 2);
			pipe_blocs->dqm = 127;
			clean_and_exit(full, pipe_blocs->dqm);
		}
		else if (env)
			*cmd = env;
	}
}

static void	control_slash_execute(char *cmd, t_general *pipe_blocs, t_full *full)
{
	struct stat	sb;

	if (stat(cmd, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		pipe_blocs->dqm = 126;
		clean_and_exit(full, pipe_blocs->dqm);
	}
	else if (access(cmd, F_OK) != 0)
	{
		error_msg(2, cmd, 0, pipe_blocs);
		pipe_blocs->dqm = 127;
		clean_and_exit(full, pipe_blocs->dqm);
	}
	else if (access(cmd, X_OK) != 0)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		pipe_blocs->dqm = 126;
		clean_and_exit(full, pipe_blocs->dqm);
	}
}

void	execute_command(t_general *pipe_blocs, t_now *get, t_env *envv,
		t_full *full)
{
	char	*args;
	char	**paths;
	int		command_found;
	char	**argv;
	char	*cmd;

	cmd = pipe_blocs->acces_args->args[0]->str;
	if (ft_strchr(cmd, '/') || cmd[0] == '.')
	{
		control_slash_execute(cmd, pipe_blocs, full);
		argv = make_argv(pipe_blocs->acces_args, envv);
		execve(cmd, argv, get->envp);
	}
	if_have_dollar_execute(&cmd, pipe_blocs, envv, full);
	command_found = 0;
	path_control_execute(&args, pipe_blocs, cmd, full);
	paths = ft_split(args, ':');
	go_to_execve_execute(paths, &command_found, full, pipe_blocs);
	if_command_not_found_execute(command_found, pipe_blocs, full, &paths);
}
