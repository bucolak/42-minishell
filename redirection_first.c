/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_first.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:51:00 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 20:00:15 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *str)
{
	int		i;
	char	*redirection[5];

	redirection[0] = "<";
	redirection[1] = "<<";
	redirection[2] = ">";
	redirection[3] = ">>";
	redirection[4] = NULL;
	i = 0;
	while (redirection[i])
	{
		if (ft_strcmp(redirection[i], str) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	renew_else_block(t_arg ***new, t_general *tmp, int *i, int *j)
{
	(*new)[*j] = malloc(sizeof(t_arg));
	(*new)[*j]->str = ft_strdup(tmp->acces_args->args[*i]->str);
	(*new)[*j]->flag = tmp->acces_args->args[*i]->flag;
	(*new)[*j]->s = tmp->acces_args->args[*i]->s;
}

static int	count_redirection_args(t_general *list)
{
	int	i;

	i = 0;
	while (list->acces_args->args[i])
		i++;
	return (i);
}

static void	process_args_array(t_arg **new, t_general *tmp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tmp->acces_args->args[i])
	{
		if (is_redirection(tmp->acces_args->args[i]->str) == 1
			&& (tmp->acces_args->args[i]->flag == 5
				|| tmp->acces_args->args[i]->flag == 2))
				{
					free(tmp->acces_args->args[i]->str);
					free(tmp->acces_args->args[i+1]->str);
					i += 2;
				}
		else
		{
			renew_else_block(&new, tmp, &i, &j);
			j++;
			i++;
		}
	}
	new[j] = NULL;
}

void	renew_block2(t_general *list)
{
	int		arg_count;
	t_arg	**new;

	arg_count = count_redirection_args(list);
	new = ft_calloc(sizeof(t_arg *), (arg_count + 1));
	process_args_array(new, list);
	if(list->acces_args->args)
		free(list->acces_args->args);
	list->acces_args->args = new;
}
