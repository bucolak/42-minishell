/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:45:21 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 20:51:20 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_quotes(const char *line, int pos)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	i = 0;
	in_quotes = 0;
	quote_type = 0;
	while (i < pos)
	{
		if ((line[i] == '"' || line[i] == '\'') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = line[i];
		}
		else if (line[i] == quote_type)
		{
			in_quotes = 0;
			quote_type = 0;
		}
		i++;
	}
	return (in_quotes);
}

void	pipe_parse(t_general **pipe_block, char *line)
{
	int			i;
	int			start;
	t_general	*tmp;
	char		*trimmed;

	i = 0;
	start = 0;
	tmp = *pipe_block;
	while (line[i])
	{
		if (line[i] == '|' && !is_in_quotes(line, i))
		{
			trimmed = ft_substr(line, start, i - start);
			tmp->blocs = ft_strtrim(trimmed, " ");
			free(trimmed);
			tmp->next = create_general_node(tmp->dqm);
			tmp = tmp->next;
			start = i + 1;
		}
		i++;
	}
	trimmed = ft_substr(line, start, i - start);
	tmp->blocs = ft_strtrim(trimmed, " ");
	free(trimmed);
}

int	has_redireciton(t_general *pipe_blocks)
{
	int	i;

	i = 0;
	while (pipe_blocks->acces_args->args[i])
	{
		if ((ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<") == 0
				|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, "<<") == 0
				|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">") == 0
				|| ft_strcmp(pipe_blocks->acces_args->args[i]->str, ">>") == 0)
			&& (pipe_blocks->acces_args->args[i]->flag == 5))
			return (1);
		i++;
	}
	return (0);
}

char	*get_getenv(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(key, tmp->key) == 0)
			return (tmp->data);
		tmp = tmp->next;
	}
	return (NULL);
}

int	has_heredoc(t_general *list)
{
	int	i;

	i = 0;
	if (!list || !list->acces_args || !list->acces_args->args)
		return (0);
	while (list->acces_args->args[i])
	{
		if (list->acces_args->args[i]->str
			&& ft_strcmp(list->acces_args->args[i]->str, "<<") == 0
			&& (list->acces_args->args[i]->flag == 2
				|| list->acces_args->args[i]->flag == 5))
			return (1);
		i++;
	}
	return (0);
}
