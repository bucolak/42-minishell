/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wq_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:07:58 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 16:31:15 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_without_quotes_parser_ifblock(t_general *a, int *i, int *j,
		int *k)
{
	char	*cleaned;
	char	*tmp_str;

	tmp_str = ft_substr(a->blocs, *j, (*i) - (*j));
	a->acces_args->args[*k] = create_arg(tmp_str, 2, 0);
	free(tmp_str);
	cleaned = clean_double_quotes(a->acces_args->args[*k]->str);
	free(a->acces_args->args[*k]->str);
	a->acces_args->args[*k]->str = cleaned;
	if (a->blocs[*i] == '\'' || a->blocs[*i] == '"' || !a->blocs[*i])
		a->acces_args->args[*k]->s = 0;
	(*k)++;
}

static void	handle_without_quotes_parser_elblock(t_general *a, int *i, int *j,
		int *k)
{
	char	*cleaned;
	char	*tmp_str;

	tmp_str = ft_substr(a->blocs, *j, (*i) - (*j));
	a->acces_args->args[*k] = create_arg(tmp_str, 2, 0);
	free(tmp_str);
	cleaned = clean_double_quotes(a->acces_args->args[*k]->str);
	free(a->acces_args->args[*k]->str);
	a->acces_args->args[*k]->str = cleaned;
	if (a->blocs[*i] == '\'' || a->blocs[*i] == '"')
		a->acces_args->args[*k]->s = 0;
	(*k)++;
}

void	handle_without_quotes_parser(t_general *a, int *i, int *j, int *k)
{
	*j = *i;
	while (a->blocs[*i] && a->blocs[*i] != ' ' && a->blocs[*i] != '\t'
		&& a->blocs[*i] != '<' && a->blocs[*i] != '>' && a->blocs[*i] != '"'
		&& a->blocs[*i] != '\'')
		(*i)++;
	if (!a->blocs[(*i) - 1] && ((a->blocs[*i] == '"' && a->blocs[(*i)
					+ 1] == '"') || (a->blocs[*i] == '\'' && a->blocs[(*i)
					+ 1] == '\'')))
	{
		(*i) += 2;
		while (a->blocs[*i] && a->blocs[*i] != ' ' && a->blocs[*i] != '\t'
			&& a->blocs[*i] != '<' && a->blocs[*i] != '>' && a->blocs[*i] != '"'
			&& a->blocs[*i] != '\'')
			(*i)++;
	}
	if ((!a->blocs[*i] || a->blocs[*i] == ' ' || a->blocs[*i] == '<'
			|| a->blocs[*i] == '>' || (a->blocs[*i] != '"'
				&& a->blocs[*i] != '\'')) && a->blocs[(*i) - 1] != '"'
		&& a->blocs[(*i) - 1] != '\'')
		handle_without_quotes_parser_ifblock(a, i, j, k);
	else
		handle_without_quotes_parser_elblock(a, i, j, k);
}
