/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:16:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/12 17:32:58 by bucolak          ###   ########.fr       */
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

void	handle_heredoc(t_general *list)
{
	int i = 0;
	char *line;
	t_heredoc *heredoc;
	t_heredoc *new_node;
	t_heredoc *tmp;
	heredoc = NULL;
	new_node = NULL;
	while (list->acces_args->args[i])
	{
		if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0)
		{
			if (!list->acces_args->args[i + 1])
            {
                ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
                return;
            }
            i++;
            while (1)
            {
                new_node = malloc(sizeof(t_heredoc));
                line = readline("> ");
                new_node->content = ft_strdup(line);
                new_node->next = NULL;
                if (ft_strcmp(line, list->acces_args->args[i]->str) == 0)
                {
                    free(line);
                    break;
                }
                free(line);

                if (!heredoc)
                {
                    heredoc = new_node;
                    tmp = new_node;
                }
                else
                {
                    tmp->next = new_node;
                    tmp = new_node;
                }
            }
            heredoc_actions(list, tmp, heredoc);
		}
		i++;
	}
}
