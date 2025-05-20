/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:12:55 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/20 20:58:04 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redireciton2(char *str)
{
    if (*str == '>' || *str == '<')
        return (1); 
    return (0);
}

void	parse_input(t_general *a)
{
	int		i;
	int		j;
	int		k;
int len;
	while (a)
	{
		k = 0;
		i = 0;
		j = 0;
		while (a->blocs[i])
		{
			while (a->blocs[i] == ' ')
				i++;
			if (!a->blocs[i])
				break ;
			if (is_redireciton2(a->blocs+i) == 1)
			{
                len = 1;
                if ((a->blocs[i] == a->blocs[i + 1]) && (a->blocs[i] == '<' || a->blocs[i] == '>'))
                    len = 2;
				a->acces_args->args[k] = create_arg(ft_substr(a->blocs, i, len), 5);
				k++;
                i+=len;
                continue;
			}
			else if (a->blocs[i] == '"')
			{
				j = ++i;
				while (a->blocs[i] != '"' && a->blocs[i])
					i++;
				if (a->blocs[i] == '"' && (!a->blocs[i+1] || a->blocs[i+1] == ' '))
				{
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 0);
					k++;
                    i++;
				}
				else
				{
                    while (a->blocs[i])
					    i++;
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 4);
					k++;
				}
                continue;
			}
            else if(a->blocs[i] == '\'')
            {
                j = ++i;
				while (a->blocs[i] != '\'' && a->blocs[i])
					i++;
				if (a->blocs[i] == '\'' && (!a->blocs[i] || a->blocs[i] == ' '))
				{
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 1);
					k++;
                    i++;
				}
				else
				{
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 4);
					k++;
				}
                continue;
            }
            else
            {
                j = i;
                while (a->blocs[i] && a->blocs[i] != ' ' && a->blocs[i] != '"' && a->blocs[i] != '\'' && a->blocs[i] != '<' && a->blocs[i] != '>')
                    i++;
                if (i > j)
                    a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, j, i - j), 2);
                else
                    i++; 
                continue;
            }
            
		}
		a = a->next;
	}
}
