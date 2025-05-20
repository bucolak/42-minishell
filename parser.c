/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:12:55 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/20 18:17:31 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_input(t_general *a)
{
	char **tmp;
	int i;
	int j;
    
	while (a)
	{
		i = 0;
		j = 0;
		tmp = ft_split(a->blocs, ' ');
		while (tmp[i])
		{
			a->acces_args->args[i] = create_arg(tmp[i], 0);
			i++;
		}
		a->acces_args->args[i] = NULL;
		i = 0;
		while (a->acces_args->args[i])
		{
			if (a->acces_args->args[i]->str[0] == '"')
			{
				j = 1;
				while (a->acces_args->args[i]->str[j]
					&& a->acces_args->args[i]->str[j] != '"')
					j++;
				if (a->acces_args->args[i]->str[j] == '"')
				{
					if (a->acces_args->args[i]->str[j + 1])
						a->acces_args->args[i]->flag = 4;
					else
						a->acces_args->args[i]->flag = 0;
				}
			}
			else if (a->acces_args->args[i]->str[0] == '\'')
			{
				j = 1;
				while (a->acces_args->args[i]->str[j]
					&& a->acces_args->args[i]->str[j] != '\'')
					j++;
				if (a->acces_args->args[i]->str[j] == '\'')
				{
					if (a->acces_args->args[i]->str[j + 1])
						a->acces_args->args[i]->flag = 4;
					else
						a->acces_args->args[i]->flag = 1;
				}
			}
			else if (ft_strcmp(a->acces_args->args[i]->str, "<") == 0
					|| ft_strcmp(a->acces_args->args[i]->str, "<<") == 0
					|| ft_strcmp(a->acces_args->args[i]->str, ">") == 0
					|| ft_strcmp(a->acces_args->args[i]->str, ">>") == 0)
				a->acces_args->args[i]->flag = 5;
			else if (a->acces_args->args[i]->str[0] != '\''
					&& a->acces_args->args[i]->str[0] != '"')
			{
				j = 0;
				while (a->acces_args->args[i]->str[j] != '\''
					&& a->acces_args->args[i]->str[j] != '"'
					&& a->acces_args->args[i]->str[j])
					j++;
				if (!a->acces_args->args[i]->str[j])
					a->acces_args->args[i]->flag = 2;
				else
					a->acces_args->args[i]->flag = 4;
			}
			i++;
		}
		free_split(tmp);
		a = a->next;
	}
}

