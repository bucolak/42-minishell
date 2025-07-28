/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:12:55 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/29 01:22:42 by buket            ###   ########.fr       */
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
int count_args(const char *str)
{
    int count = 0;
    int i = 0;

    while (str[i])
    {
        while (str[i] == ' ')
            i++;
        if (!str[i])
            break;
        if (str[i] == '<' || str[i] == '>')
        {
            count++;
            if (str[i + 1] == str[i])
                i += 2;
            else
                i++;
        }
        else if (str[i] == '"' || str[i] == '\'')
        {
            char quote = str[i++];
            count++;
            while (str[i] && str[i] != quote)
                i++;
            if (str[i] == quote)
                i++;
        }
        else
        {
            count++;
            while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>' && str[i] != '"' && str[i] != '\'')
                i++;
        }
    }
    return count;
}

void	parse_input(t_general *a)
{
	int	i;
	int f;
	int	j;
	int	k;
	int	len;
	char *tmp_str;
	int s;
	f = 0;
	while (f<count_args(a->blocs))
	{
		k = 0;
		i = 0;
		j = 0;
		a->acces_args->args = ft_calloc(count_args(a->blocs)+1, sizeof(t_arg *));
		//a->acces_args->args[count_args(a->blocs)] = NULL;
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
				s = 1;
				if(a->blocs[i] == '\'' || a->blocs[i] == '"')
						s = 0;
				a->acces_args->args[k] = create_arg(ft_substr(a->blocs, i, len),
						5, 0,s);
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
					s = 1;
					if(a->blocs[i] == '\'' || a->blocs[i] == '"')
						s = 0;
					a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 0, 0, s);
					k++;
					i++;
				}
				else
				{
					while (a->blocs[i])
						i++;
					if(has_single(&a->blocs[j]) == 0)
					{
						s = 1;
						if(a->blocs[i] == '\'' || a->blocs[i] == '"')
							s = 0;
						a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
								- j), 0,0, s);
						a->acces_args->args[k]->str = clean_double_quotes(a->acces_args->args[k]->str);
					}
						
					// else
					// {
					// 	a->acces_args->args[k] = create_arg(ft_substr(a->blocs, j, i
					// 			- j), 4, 0);
						
					// }
							
					k++;
				}
				continue ;
			}
			else if (a->blocs[i] == '\'')
			{
				j = ++i;
				while (a->blocs[i] != '\'' && a->blocs[i])
					i++;
				if (a->blocs[i] == '\'' && (!a->blocs[i+1] || a->blocs[i+1] == ' ' || a->blocs[i-1] == '\''))
				{
					tmp_str = ft_substr(a->blocs, j, i- j);
					if(tmp_str[0])
					{
						s = 1;
						if(a->blocs[i] == '\'' || a->blocs[i] == '"')
							s = 0;
						a->acces_args->args[k++] = create_arg(tmp_str, 1, 0, s);
					}
					//free(tmp_str);
					i++;
				}
				// else
				// {
				// 	tmp_str = ft_substr(a->blocs, j, i- j);
				// 	if(tmp_str[0])
				// 		a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, j, i- j), 4, 0);
				// 	else
				// 		i++;
				// 	//free(tmp_str);
				// }
				continue ;
			}
			else
			{
				j = i;
				while (a->blocs[i] && a->blocs[i] != ' ' && a->blocs[i] != '<'
					&& a->blocs[i] != '>' && a->blocs[i] != '"' && a->blocs[i] != '\'')
					i++;
				if ((!a->blocs[i] || a->blocs[i] == ' ' || a->blocs[i] == '<' || a->blocs[i] == '>' || a->blocs[i] != '"' || a->blocs[i] != '\'') && a->blocs[i-1] != '"' && a->blocs[i-1] != '\'')
				{
					if(a->blocs[i] == '\'' || a->blocs[i] == '"')
						s = 0;
					a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, j,i - j), 2, 0, s);
				}
				// else if(!a->blocs[i] && (a->blocs[i-1] == '"' || a->blocs[i-1] == '\''))
				// {
				// 	a->acces_args->args[k++] = create_arg(ft_substr(a->blocs, j,
				// 				i - j), 4, 4);
				// }
				continue ;
			}
		}
		a->acces_args->args[k] = NULL;
		if(a->next)
			a = a->next;
		f++;
	}
}
