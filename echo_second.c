/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:24:36 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 14:21:24 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_exit_code(t_general *tmp, int *j)
{
	char	*new;

	new = ft_itoa(tmp->dqm);
	ft_putstr_fd(new, 1);
	free(new);
	tmp->dqm = 0;
	(*j) += 2;
}

static void	handle_env_variable(char *str, int *j)
{
	int		start;
	char	*new;

	start = ++(*j);
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		(*j)++;
	if (start == *j)
	{
		(*j)--;
		ft_putchar_fd(str[*j], 1);
		(*j)++;
		return ;
	}
	new = ft_substr(str, start, (*j) - start);
	if (!new)
		ft_putchar_fd(' ', 1);
	free(new);
}

static void	handle_quotes_and_chars(char *str, t_general *tmp, int *j, int i)
{
	if (str[*j] == '"' && str[*j + 1] == '"' && tmp->acces_args->args[i + 1])
	{
		ft_putchar_fd(' ', 1);
		(*j) += 2;
	}
	else if (str[*j] == '"' && str[*j + 1] == '"'
		&& !tmp->acces_args->args[i + 1])
		(*j) += 2;
	else if (str[*j] != '"')
	{
		ft_putchar_fd(str[*j], 1);
		(*j)++;
	}
}

static void	echo_flag_0_and_2_second(char *str, t_general *tmp, int *j, int i)
{
	if (str[*j] == '$' && str[*j + 1])
	{
		if (str[*j + 1] == '?')
			handle_exit_code(tmp, j);
		else
			handle_env_variable(str, j);
	}
	else
		handle_quotes_and_chars(str, tmp, j, i);
}

void	echo_flag_0_and_2(char *str, t_general *tmp, int i)
{
	int	j;

	j = 0;
	while (str[j])
	{
		echo_flag_0_and_2_second(str, tmp, &j, i);
	}
	if (tmp->acces_args->args[i + 1] && ft_strcmp(tmp->acces_args->args[i]->str,
			"\"\"") != 0)
	{
		ft_putchar_fd(' ', 1);
	}
}