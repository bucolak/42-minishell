/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtin_scnd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:12:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/25 14:52:37 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *str)
{
	int		i;
	char	*builtin[9];

	builtin[0] = "echo";
	builtin[1] = "pwd";
	builtin[2] = "cd";
	builtin[3] = "env";
	builtin[4] = "export";
	builtin[5] = "unset";
	builtin[6] = "exit";
	builtin[7] = "$?";
	builtin[8] = NULL;
	i = 0;
	while (builtin[i])
	{
		if (ft_strcmp(builtin[i], str) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	pwd_cmd(char **ar, t_general *list, t_env *env)
{
	char	*line;
	char	*new;

	if (ft_strncmp(ar[0], "pwd", 3) == 0)
	{
		line = getcwd(NULL, 0);
		list->dqm = 0;
		if (!line)
		{
			new = get_getenv(env, "PWD");
			ft_putstr_fd(new, 1);
			ft_putchar_fd('\n', 1);
			free(line);
			list->dqm = 1;
			return ;
		}
		printf("%s\n", line);
		free(line);
		list->dqm = 0;
	}
}

void	update_cd_helper_scnd(char *cwd, t_env *tmp)
{
	if (tmp->data)
		free(tmp->data);
	if (cwd)
		tmp->data = cwd;
	else
		tmp->data = NULL;
}
