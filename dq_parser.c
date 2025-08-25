/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dq_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:09:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/25 21:38:50 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_double_quotes_parser_ifblocks(t_general *a, int *i, int *j,
		int *k)
{
	char	*tmp_str;

	tmp_str = ft_substr(a->blocs, *j, (*i) - (*j));
	if (!tmp_str[0])
	{
		free(tmp_str);
		tmp_str = ft_substr(a->blocs, (*j) - 1, 2);
		a->acces_args->args[*k] = create_arg(tmp_str, 0);
		free(tmp_str);
		if (a->blocs[(*i) + 1] && a->blocs[(*i) + 1] != ' ')
			a->acces_args->args[(*k)]->s = 0;
		(*k)++;
	}
	else
	{
		a->acces_args->args[*k] = create_arg(tmp_str, 0);
		free(tmp_str);
		if ((a->blocs[(*i) + 1] == '\'' || a->blocs[(*i) + 1] == '"'
				|| a->blocs[(*i) + 1] != ' ') && a->acces_args->args[*k])
			a->acces_args->args[*k]->s = 0;
		(*k)++;
	}
	(*i)++;
}

int	handle_double_quotes_parser(t_general *a, int *i, int *j, int *k)
{
	*j = ++(*i);
	while (a->blocs[*i] && a->blocs[*i] != '"')
		(*i)++;
	if (a->blocs[*i] == '"')
		handle_double_quotes_parser_ifblocks(a, i, j, k);
	else
	{
		ft_putstr_fd("Syntax Error: The quote did not close\n", 2);
		return (1);
	}
	return (0);
}
