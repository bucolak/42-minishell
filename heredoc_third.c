/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_third.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:41:22 by seerel            #+#    #+#             */
/*   Updated: 2025/08/23 13:27:22 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_syntax_error(void)
{
	ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
	return (1);
}

static int	handle_heredoc_token(t_general *tmp, t_full *full, t_hdoc_state *st)
{
	tmp->flag_heredoc = 1;
	if (!tmp->acces_args->args[st->i + 1])
		return (handle_syntax_error());
	if (handle_heredoc_process(tmp, full, &st->j, &st->status))
		return (1);
	st->j++;
	st->i += 2;
	return (0);
}

int	process_heredocs(t_general *tmp, t_full *full)
{
	t_hdoc_state	st;

	st.i = 0;
	st.j = 0;
	st.status = 0;
	while (tmp->acces_args->args[st.i])
	{
		if (ft_strcmp(tmp->acces_args->args[st.i]->str, "<<") == 0)
		{
			if (handle_heredoc_token(tmp, full, &st))
				return (1);
		}
		else
			st.i++;
	}
	remove_heredoc(tmp);
	return (0);
}