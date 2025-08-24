/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtin_scnd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:12:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 14:13:18 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *str)
{
	int		i;
	char	*builtin[] = {"echo", "pwd", "cd", "env", "export", "unset", "exit",
			"$?", NULL};

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

	if (ft_strncmp(ar[0], "pwd", 3) == 0)
	{
		line = getcwd(NULL, 0);
		list->dqm = 0;
		if (!line)
		{
			printf("Error\n");
			line = get_getenv(env, "HOME");
			free(line);
			list->dqm = 1;
			return ;
		}
		printf("%s\n", line);
		free(line);
		list->dqm = 0;
	}
}
