/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_null.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seerel <seerel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:07:26 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 19:30:39 by seerel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_remove_null(char *str)
{
	int	c;
	int	j;

	c = 0;
	j = 0;
	while (str[j])
	{
		if ((str[j] == '"' && str[j + 1] && str[j + 1] == '"' && ((j > 0
						&& str[j - 1]) || str[j + 2])) || (str[j] == '\''
				&& str[j + 1] && str[j + 1] == '\'' && ((j > 0 && str[j - 1])
					|| str[j + 2])))
		{
			j += 2;
		}
		else
		{
			c++;
			j++;
		}
	}
	return (c + 1);
}

static int	is_empty_quote_pair(char *str, int j)
{
	return ((str[j] == '"' && str[j + 1] && str[j + 1] == '"'
			&& ((j > 0 && str[j - 1]) || str[j + 2]))
		|| (str[j] == '\'' && str[j + 1] && str[j + 1] == '\''
			&& ((j > 0 && str[j - 1]) || str[j + 2])));
}

static char	*process_string(char *str)
{
	char	*new;
	int		j;
	int		k;

	new = malloc(sizeof(char) * count_remove_null(str));
	if (!new)
		return (NULL);
	j = 0;
	k = 0;
	while (str[j])
	{
		if (is_empty_quote_pair(str, j))
			j += 2;
		else
			new[k++] = str[j++];
	}
	new[k] = '\0';
	return (new);
}

void	remove_null(t_general *list)
{
	t_general	*tmp;
	char		*new;
	int			i;

	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			new = process_string(tmp->acces_args->args[i]->str);
			if (!new)
				return ;
			if (tmp->acces_args->args[i]->str)
				free(tmp->acces_args->args[i]->str);
			tmp->acces_args->args[i]->str = new;
			i++;
		}
		tmp = tmp->next;
	}
}
