/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:16:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/06/01 16:07:37 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_heredoc(t_general *list)
{
	int		i;
	int		j;
	t_arg	**new_arg;

	i = 0;
	while (list->acces_args->args[i])
	{
		i++;
	}
	new_arg = malloc(sizeof(t_arg *) * (i + 1));
	i = 0;
	j = 0;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
		{
			i += 2; // skip '<<' and its delimiter
			continue ;
		}
		new_arg[j++] = list->acces_args->args[i++];
	}
	new_arg[j] = NULL;
	list->acces_args->args = new_arg;
}

void	handle_heredoc(t_general *list)
{
	int		i;
	char	*line;
	int		fd[2];

	i = 0;
	while (list)
	{
		i = 0;
		while (list->acces_args->args[i])
		{
			if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
			{
				if (!list->acces_args->args[i + 1])
				{
					ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
									2);
					return ;
				}
				i++;
				pipe(fd);
				while (1)
				{
					line = readline(">");
					if (ft_strcmp(line, list->acces_args->args[i]->str) != 0)
					{
						ft_putendl_fd(line, fd[1]);
					}
					else
						break ;
				}
				close(fd[1]);
				if (list->heredoc_fd != -1)
					close(list->heredoc_fd);
				list->heredoc_fd = fd[0];
			}
			i++;
		}
		remove_heredoc(list);
		list = list->next;
	}
}
