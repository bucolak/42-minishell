/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_count_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seerel <seerel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:14:09 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 19:30:21 by seerel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_skip_redirection(t_arg **args, int i)
{
	return (is_redirection(args[i]->str) == 1 && args[i]->flag != 0
		&& args[i]->flag != 1 && is_redirection(args[i + 1]->str) == 0
		&& (args[i + 1]->flag == 0 || args[i + 1]->flag == 1));
}

char	*create_combined_string(t_arg *arg1, t_arg *arg2)
{
	char	*new;
	int		c;

	c = ft_strlen(arg1->str) + ft_strlen(arg2->str);
	new = malloc(sizeof(char) * (c + 1));
	if (!new)
		return (NULL);
	ft_strlcpy(new, arg1->str, c + 1);
	ft_strlcat(new, arg2->str, c + 1);
	return (new);
}

void	update_flags(t_arg **args, int i, char *new)
{
	if ((ft_strcmp(new, "<<") == 0 || ft_strcmp(new, ">>") == 0)
		&& ((args[i]->flag != 5 && args[i]->flag != 2)
			|| (args[i + 1]->flag != 5 && args[i + 1]->flag != 2)))
		args[i]->flag = 0;
	if (args[i + 1] && is_redirection(args[i + 1]->str) == 1
		&& args[i + 1]->flag != 5 && args[i + 1]->s == 0)
		args[i]->flag = 6;
}

void	remove_arg_from_array(t_arg **args, int i)
{
	int	j;

	free(args[i + 1]->str);
	free(args[i + 1]);
	j = i + 1;
	while (args[j])
	{
		args[j] = args[j + 1];
		j++;
	}
	args[j] = NULL;
}

void	process_adjacent_args(t_arg **args, int i)
{
	char	*new;

	new = create_combined_string(args[i], args[i + 1]);
	if (!new)
		return ;
	update_flags(args, i, new);
	args[i]->s = args[i + 1]->s;
	remove_arg_from_array(args, i);
	free(args[i]->str);
	args[i]->str = new;
}
