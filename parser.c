/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:12:55 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/16 00:42:49 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redireciton2(char *str)
{
	if (*str == '>' || *str == '<')
		return (1);
	return (0);
}
int has_single(char *str)
{
	int i = 0;
	while(str[i] && str[i] != ' ')
	{
		if(str[i] == '\'')
			return 1;
		i++;
	}
	return 0;
}
char *clean_double_quotes(char *str)
{
	int i = 0;
	int j = 0;
	char *new;
	while(str[i] && str[i] != ' ')
	{
		if(str[i] == '"')
			j++;
		i++;
	}
	new = malloc(sizeof(char) * (i-j+1));
	i = 0;
	j = 0;
	while(str[i] && str[i] != ' ')
	{
		if(str[i] != '"')
			new[j++] = str[i];
		i++;
	}
	new[j] = '\0';
	return new;
}
void	parse_input(t_general *a)
{
	int	i;
	int	j;
	int	k;
	int	len;

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
			if (is_redireciton2(a->blocs + i) == 1)
			{
				len = 1;
				if ((a->blocs[i] == a->blocs[i + 1]) && (a->blocs[i] == '<'
						|| a->blocs[i] == '>'))
					len = 2;
				a->acces_args->args[k] = create_arg(ft_substr(a->blocs, i, len),
						5, 0);
				k++;
				i += len;
				continue ;
			}
			else if (a->blocs[i] == '"')
			{
				j = ++i;
				while (a->blocs[i] != '"' && a->blocs[i])
					i++;
				if (a->blocs[i] == '"' && (!a->blocs[i + 1] || a->blocs[i
					+ 1] == ' '))
				{
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 0, 0);
					k++;
					i++;
				}
				else
				{
					while (a->blocs[i])
						i++;
					if(has_single(&a->blocs[j]) == 0)
					{
						a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 0, 0);
						a->acces_args->args[k]->str = clean_double_quotes(a->acces_args->args[k]->str);
					}
						
					else
					{
						a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 4, 0);
						
					}
							
					k++;
				}
				continue ;
			}
			else if (a->blocs[i] == '\'')
			{
				j = ++i;
				while (a->blocs[i] != '\'' && a->blocs[i])
					i++;
				if (a->blocs[i] == '\'' && (!a->blocs[i+1] || a->blocs[i+1] == ' '))
				{
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 1, 0);
					k++;
					i++;
				}
				else
				{
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 4, 0);
					k++;
				}
				continue ;
			}
			else
			{
				j = i;
				while (a->blocs[i] && a->blocs[i] != ' ' && a->blocs[i] != '<'
					&& a->blocs[i] != '>')
					i++;
				if ((!a->blocs[i] || a->blocs[i] == ' ' || a->blocs[i] == '<' || a->blocs[i] == '>') && a->blocs[i-1] != '"' && a->blocs[i-1] != '\'')
				{
					a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, j,i - j), 2, 0);
				}
				else if(!a->blocs[i] && (a->blocs[i-1] == '"' || a->blocs[i-1] == '\''))
				{
					a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, j,
								i - j), 4, 4);
				}
				continue ;
			}
		}
		a = a->next;
	}
}
