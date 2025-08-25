/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:25:16 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/25 14:54:00 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_cd_helper(t_env *tmp, t_env *env)
{
	char	*cwd;

	while (tmp)
	{
		if (ft_strcmp(tmp->key, "OLDPWD") == 0)
		{
			if (tmp->data)
				free(tmp->data);
			tmp->data = ft_strdup(get_getenv(env, "PWD"));
			break ;
		}
		tmp = tmp->next;
	}
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PWD") == 0)
		{
			cwd = getcwd(NULL, 0);
			update_cd_helper_scnd(cwd, tmp);
			break ;
		}
		tmp = tmp->next;
	}
}

static void	cd_helper(t_arg **args, char *env_name, t_general *pipe_blocks,
		t_env *env)
{
	t_env	*tmp;

	tmp = env;
	if (args[1]->str[0] == '$' && (args[1]->flag == 0 || args[1]->flag == 2))
	{
		env_name = args[1]->str + 1;
		chdir(get_getenv(env, env_name));
		return ;
	}
	else if (chdir(args[1]->str) == -1)
	{
		perror("cd");
		pipe_blocks->dqm = 1;
	}
	update_cd_helper(tmp, env);
}

static void	cd_dash_control_scnd(t_env *tmp, char *new_oldpwd, char *old_pwd)
{
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "OLDPWD") == 0)
		{
			if (tmp->data)
				free(tmp->data);
			tmp->data = new_oldpwd;
		}
		else if (ft_strcmp(tmp->key, "PWD") == 0)
		{
			if (tmp->data)
				free(tmp->data);
			tmp->data = ft_strdup(old_pwd);
		}
		tmp = tmp->next;
	}
}

static void	cd_dash_control(t_env *env)
{
	char	*old_pwd;
	char	*pwd;
	char	*new_oldpwd;
	t_env	*tmp;

	old_pwd = ft_strdup(get_getenv(env, "OLDPWD"));
	pwd = ft_strdup(get_getenv(env, "PWD"));
	new_oldpwd = ft_strdup(pwd);
	tmp = env;
	cd_dash_control_scnd(tmp, new_oldpwd, old_pwd);
	if (pwd)
		free(pwd);
	if (old_pwd)
		free(old_pwd);
	ft_putstr_fd(new_oldpwd, 1);
	ft_putchar_fd('\n', 1);
	if (get_getenv(env, "OLDPWD"))
		chdir(get_getenv(env, "OLDPWD"));
}

void	cd_cmd(t_arg **args, t_env *env, t_general *pipe_blocks)
{
	char	*line;
	char	*env_name;

	env_name = NULL;
	if (!args[1] || ft_strcmp(args[1]->str, "~") == 0)
	{
		line = get_getenv(env, "HOME");
		if (line)
			chdir(line);
	}
	else if (ft_strcmp(args[1]->str, "-") == 0)
		cd_dash_control(env);
	else if (args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		pipe_blocks->dqm = 1;
	}
	else
		cd_helper(args, env_name, pipe_blocks, env);
}
