/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:16:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/31 06:09:52 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_actions(t_general *list, t_heredoc *tmp, t_heredoc *heredoc)
{
	t_heredoc	*to_free;

	tmp = heredoc;
	if (ft_strcmp(list->acces_args->args[0]->str, "cat") == 0)
	{
		while (tmp)
		{
			ft_putstr_fd(tmp->content, 1);
			ft_putchar_fd('\n', 1);
			tmp = tmp->next;
		}
	}
	renew_block2(list);
	tmp = heredoc;
	while (tmp)
	{
		to_free = tmp;
		tmp = tmp->next;
		free(to_free->content);
		free(to_free);
	}
	heredoc = NULL;
}

t_heredoc	*create_heredoc_node(char *line)
{
	t_heredoc	*new_node;

	new_node = malloc(sizeof(t_heredoc));
	new_node->content = ft_strdup(line);
	new_node->next = NULL;
	return (new_node);
}

void	append_heredoc_node(t_heredoc **head, t_heredoc **tail,
		t_heredoc *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

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
			list->heredoc_fd = fd[0];
			// dup2(fd[0], 0);
			// close(fd[0]);
		}
		i++;
	}
	remove_heredoc(list);
}
