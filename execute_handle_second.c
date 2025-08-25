/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_handle_second.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:09:26 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/25 15:00:09 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_malloc_execute(t_general *list, int j)
{
	int		c;
	int		i;
	char	*str;
	char	*itoa;

	i = 0;
	c = 0;
	if (list->acces_args->args[j])
	{
		str = list->acces_args->args[j]->str;
		while (str[i])
		{
			if (str[i] == '$' && str[i + 1] && str[i + 1] == '?')
			{
				itoa = ft_itoa(list->dqm);
				c += ft_strlen(itoa);
				free(itoa);
				i++;
			}
			else
				c++;
			i++;
		}
	}
	return (c);
}

static void	expand_dollar_qmark_execute_scnd(t_general *list, char **new,
		char *str)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] == '?')
		{
			ft_memcpy(*new + k, ft_itoa(list->dqm),
				ft_strlen(ft_itoa(list->dqm)));
			i += 2;
			k += ft_strlen(ft_itoa(list->dqm));
		}
		else
			(*new)[k++] = str[i++];
	}
	(*new)[k] = '\0';
}

void	expand_dollar_qmark_execute(t_general *list)
{
	char	*str;
	int		j;
	char	*new;

	j = 0;
	while (list->acces_args->args[j])
	{
		new = malloc(sizeof(char) * (count_malloc_execute(list, j) + 1));
		str = list->acces_args->args[j]->str;
		expand_dollar_qmark_execute_scnd(list, &new, str);
		free(list->acces_args->args[j]->str);
		list->acces_args->args[j]->str = ft_strdup(new);
		free(new);
		j++;
	}
}
