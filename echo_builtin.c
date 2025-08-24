/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:06:08 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 20:13:18 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_n_flags(t_arg **args, int *i)
{
	int	new_line;

	new_line = 1;
	while (args[*i] && cont_n(args[*i]->str))
	{
		new_line = 0;
		(*i)++;
	}
	return (new_line);
}

static int	is_redirection_token(char *s)
{
	if (!s)
		return (0);
	if (ft_strcmp(s, ">>") == 0 || ft_strcmp(s, ">") == 0 || ft_strcmp(s,
			"<") == 0 || ft_strcmp(s, "<<") == 0)
		return (1);
	return (0);
}

static void	process_echo_token(t_general *tmp, char *s, int idx)
{
	if (tmp->acces_args->args[idx]->flag == 1)
		echo_flag_1(tmp, idx);
	else if (tmp->acces_args->args[idx]->flag == 0
		|| tmp->acces_args->args[idx]->flag == 2)
		echo_flag_0_and_2(s, tmp, idx);
}

static void	process_echo_args(t_general *tmp, t_arg **args, int *ip)
{
	char	*s;
	int		i;

	i = *ip;
	while (args[i])
	{
		if (!args[i]->str)
			i++;
		else if (ft_strcmp(args[i]->str, "-n") == 0 && args[i + 1])
			i++;
		else if (ft_strcmp(args[i]->str, "-n") == 0)
			break ;
		else if (is_redirection_token(args[i]->str))
			break ;
		else
		{
			s = args[i]->str;
			process_echo_token(tmp, s, i);
			i++;
		}
	}
	*ip = i;
}

void	echo_cmd(t_general *tmp, int i)
{
	int		new_line;
	t_arg	**args;

	args = tmp->acces_args->args;
	new_line = skip_n_flags(args, &i);
	process_echo_args(tmp, args, &i);
	if (new_line)
		ft_putchar_fd('\n', 1);
	tmp->dqm = 0;
}
