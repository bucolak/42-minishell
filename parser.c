/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:12:55 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/25 15:28:16 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_old_block(t_general *a)
{
	int	idx;

	if (a->acces_args->args)
	{
		idx = 0;
		while (a->acces_args->args[idx])
		{
			if (a->acces_args->args[idx]->str)
				free(a->acces_args->args[idx]->str);
			free(a->acces_args->args[idx]);
			idx++;
		}
		free(a->acces_args->args);
		a->acces_args->args = NULL;
	}
}

void	first_control_parser(t_general *a, int *i, int *k)
{
	int		len;
	char	*tmp_str;

	len = 1;
	if ((a->blocs[*i] == a->blocs[*i + 1]) && (a->blocs[*i] == '<'
			|| a->blocs[*i] == '>'))
		len = 2;
	tmp_str = ft_substr(a->blocs, *i, len);
	a->acces_args->args[*k] = create_arg(tmp_str, 5, 0);
	free(tmp_str);
	(*i) += len;
	if (a->blocs[*i] == '\'' || a->blocs[*i] == '"')
		a->acces_args->args[*k]->s = 0;
	(*k)++;
}

static int	control_while_loop_parser(t_general *a, int *i, int *j, int *k)
{
	if (is_redireciton2(a->blocs + *i) == 1)
	{
		first_control_parser(a, i, k);
		return (1);
	}
	else if (a->blocs[*i] == '"')
	{
		if (handle_double_quotes_parser(a, i, j, k) == 1)
			return (2);
		return (1);
	}
	else if (a->blocs[*i] == '\'')
	{
		if (handle_single_quotes_parser(a, i, j, k) == 1)
			return (2);
		return (1);
	}
	else
	{
		handle_without_quotes_parser(a, i, j, k);
		return (1);
	}
	return (0);
}

static void	parse_input_second(t_general **a, int *i, int *j, int *k)
{
	*k = 0;
	*i = 0;
	*j = 0;
	free_old_block(*a);
	(*a)->acces_args->args = ft_calloc(count_args((*a)->blocs) + 1,
			sizeof(t_arg *));
}

int	parse_input(t_general *a)
{
	int	i;
	int	j;
	int	k;
	int	result;

	while (a)
	{
		parse_input_second(&a, &i, &j, &k);
		while (a->blocs[i])
		{
			while (a->blocs[i] == ' ' || a->blocs[i] == '\t')
				i++;
			if (!a->blocs[i])
				break ;
			result = control_while_loop_parser(a, &i, &j, &k);
			if (result == 1)
				continue ;
			if (result == 2)
				return (1);
		}
		a->acces_args->args[k] = NULL;
		a = a->next;
	}
	return (0);
}
