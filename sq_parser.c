/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sq_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:09:01 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 16:31:46 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_single_quotes_parser_ifblock(t_general *a, int *i, int *j,
		int *k)
{
	char	*tmp_str;

	tmp_str = ft_substr(a->blocs, *j, (*i) - (*j));
	if (!tmp_str[0] && (a->blocs[(*i) + 1] == ' ' || !a->blocs[(*i) + 1]))
	{
		free(tmp_str);
		tmp_str = ft_substr(a->blocs, (*j) - 1, 2);
		a->acces_args->args[*k] = create_arg(tmp_str, 1, 0);
		free(tmp_str);
		(*k)++;
	}
	else
	{
		a->acces_args->args[*k] = create_arg(tmp_str, 1, 0);
		free(tmp_str);
		if ((a->blocs[(*i) + 1] == '\'' || a->blocs[(*i) + 1] == '"'
				|| a->blocs[(*i) + 1] != ' ') && a->acces_args->args[*k])
			a->acces_args->args[*k]->s = 0;
		(*k)++;
	}
	(*i)++;
}

static void	handle_single_quotes_parser_elblock(t_general *a, int *i, int *j,
		int *k)
{
	char	*tmp_str;
	char	*cleaned;

	if (a->blocs[*i] == '\'')
	{
		tmp_str = ft_substr(a->blocs, *j, (*i) - (*j));
		a->acces_args->args[*k] = create_arg(tmp_str, 1, 0);
		free(tmp_str);
		cleaned = clean_double_quotes(a->acces_args->args[*k]->str);
		free(a->acces_args->args[*k]->str);
		a->acces_args->args[*k]->str = cleaned;
		if ((a->blocs[(*i) + 1] == '\'' || a->blocs[(*i) + 1] == '"'
				|| a->blocs[(*i) + 1] != ' ') && a->acces_args->args[*k])
			a->acces_args->args[*k]->s = 0;
		(*k)++;
		(*i)++;
	}
}

void	handle_single_quotes_parser(t_general *a, int *i, int *j, int *k)
{
	*j = ++(*i);
	while (a->blocs[*i] != '\'' && a->blocs[*i])
		(*i)++;
	if (a->blocs[*i] == '\'' && ((a->blocs[(*i) + 1] && a->blocs[(*i)
					+ 1] != '\'') || !a->blocs[(*i) + 1]))
	{
		handle_single_quotes_parser_ifblock(a, i, j, k);
	}
	else
	{
		while (a->blocs[*i])
		{
			if (a->blocs[*i] == '\'' && a->blocs[(*i) + 1] && a->blocs[(*i)
					+ 1] == '\'')
				(*i) += 2;
			else if (!a->blocs[(*i) + 1])
				break ;
			else
				(*i)++;
		}
		handle_single_quotes_parser_elblock(a, i, j, k);
	}
}
