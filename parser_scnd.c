/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_scnd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:22:24 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/25 21:37:27 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redireciton_two(char *str)
{
	if (*str == '>' || *str == '<')
		return (1);
	return (0);
}

int	has_single(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

static void	count_args_scnd(int type, const char *str, int *i, int *count)
{
	int	quote;
	int	start;

	if (type == 0)
	{
		(*count)++;
		if (str[(*i) + 1] == str[*i])
			(*i) += 2;
		else
			(*i)++;
	}
	else if (type == 1)
	{
		quote = str[(*i)++];
		start = *i;
		while (str[*i] && str[*i] != quote)
			(*i)++;
		if ((*i) >= start)
			(*count)++;
		if (str[*i] == quote)
			(*i)++;
	}
}

int	count_args(const char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		if (!str[i])
			break ;
		if (str[i] == '<' || str[i] == '>')
			count_args_scnd(0, str, &i, &count);
		else if (str[i] == '"' || str[i] == '\'')
			count_args_scnd(1, str, &i, &count);
		else
		{
			count++;
			while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '<'
				&& str[i] != '>' && str[i] != '"' && str[i] != '\'')
				i++;
		}
	}
	return (count);
}

char	*clean_double_quotes(char *str)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '"')
			j++;
		i++;
	}
	new = malloc(sizeof(char) * (i - j + 1));
	i = 0;
	j = 0;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] != '"' && str[i] != '\'')
			new[j++] = str[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}
