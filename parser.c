/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:12:55 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/13 17:08:50 by bucolak          ###   ########.fr       */
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
		if(str[i] != '"' && str[i] != '\'')
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
        while (str[i] && (str[i] == ' ' || str[i] == '\t'))
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
            int start = i;
            while (str[i] && str[i] != quote)
                i++;
            if (i >= start) // Tırnaklar arasında bir şey varsa
                count++;
            if (str[i] == quote)
                i++;
        }
        else
        {
            count++;
            while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '<' && str[i] != '>' && str[i] != '"' && str[i] != '\'')
                i++;
        }
    }
    return count;
}

void	parse_input(t_general *a)
{
	int	i;
	int	j;
	int	k;
	int	len;
	char *cleaned;
	char *tmp_str;
	while (a)
	{
		k = 0;
		i = 0;
		j = 0;
		if (a->acces_args->args) {
            int idx = 0;
            while (a->acces_args->args[idx]) {
                if (a->acces_args->args[idx]->str)
                    free(a->acces_args->args[idx]->str);
                free(a->acces_args->args[idx]);
                idx++;
            }
            free(a->acces_args->args);
            a->acces_args->args = NULL;
        }
		a->acces_args->args = ft_calloc(count_args(a->blocs)+1, sizeof(t_arg *));
		while (a->blocs[i])
		{
			while (a->blocs[i] == ' ' || a->blocs[i] == '\t')
				i++;
			if (!a->blocs[i])
				break ;
			if (is_redireciton2(a->blocs + i) == 1)
			{
				len = 1;
				if ((a->blocs[i] == a->blocs[i + 1]) && (a->blocs[i] == '<'
						|| a->blocs[i] == '>'))
					len = 2;
				tmp_str = ft_substr(a->blocs, i, len);
				a->acces_args->args[k] = create_arg(tmp_str,5, 0);
				free(tmp_str);
				i += len;
				if(a->blocs[i] == '\'' || a->blocs[i] == '"')
				{
					a->acces_args->args[k]->s = 0;
				}
				k++;
				continue ;
			}
			else if (a->blocs[i] == '"')
			{
				j = ++i;
				while (a->blocs[i] && a->blocs[i] != '"')
				i++;
				if (a->blocs[i] == '"'  )
				{
					tmp_str = ft_substr(a->blocs, j, i- j);
					if(!tmp_str[0] )//BAK:  && (a->blocs[i+1] == ' ' || !a->blocs[i+1]) bu kontrol vardı sildim
					{
						free(tmp_str);
						tmp_str = ft_substr(a->blocs, j-1, 2);
						a->acces_args->args[k] = create_arg(tmp_str, 0, 0);
						free(tmp_str);
						if(a->blocs[i+1] && a->blocs[i+1] != ' ')
							a->acces_args->args[k]->s = 0;
						k++;
					}
					else
					{
						a->acces_args->args[k] = create_arg(tmp_str, 0, 0);
						free(tmp_str);
						if((a->blocs[i+1] == '\'' || a->blocs[i+1] == '"' || a->blocs[i+1]!=' ') && a->acces_args->args[k])
						{
							a->acces_args->args[k]->s = 0;
						}
						k++;
					}
					i++;
				}
				else
				{
					while (a->blocs[i])
					{
						if(a->blocs[i] == '"' &&  a->blocs[i+1]&&a->blocs[i+1] == '"')
							i+=2;
						else if(!a->blocs[i+1])
							break;
						else
							i++;
					}
					if (a->blocs[i] == '"')
					{
						tmp_str = ft_substr(a->blocs, j, i- j);
						if(tmp_str[0])
						{
							a->acces_args->args[k] = create_arg(tmp_str, 0, 0);
							free(tmp_str);
							cleaned = clean_double_quotes(a->acces_args->args[k]->str);
							free(a->acces_args->args[k]->str);
							a->acces_args->args[k]->str = cleaned;
							if((a->blocs[i+1] == '\'' || a->blocs[i+1] == '"' || a->blocs[i+1]!=' ') && a->acces_args->args[k])
							{
								a->acces_args->args[k]->s = 0;
							}
							k++;
						}
						else
							free(tmp_str);
						i++;
					}
				}
				continue ;
			}
			else if (a->blocs[i] == '\'')
			{
				j = ++i;
				while (a->blocs[i] != '\'' && a->blocs[i])
					i++;
					
				if (a->blocs[i] == '\''  && ((a->blocs[i+1]&&a->blocs[i+1] != '\'') || !a->blocs[i+1]))  
				{
					tmp_str = ft_substr(a->blocs, j, i- j);
					if(!tmp_str[0] && (a->blocs[i+1] == ' ' || !a->blocs[i+1]))
					{
						free(tmp_str);
						tmp_str = ft_substr(a->blocs, j-1, 2);
						a->acces_args->args[k] = create_arg(tmp_str, 1, 0);
						k++;
					}
					else
					{
						a->acces_args->args[k] = create_arg(tmp_str, 1, 0);
						free(tmp_str);
						if((a->blocs[i+1] == '\'' || a->blocs[i+1] == '"' || a->blocs[i+1]!=' ') && a->acces_args->args[k])
						{
							a->acces_args->args[k]->s = 0;
						}
						k++;
					}
					// else
					// 	free(tmp_str);
					i++;
				}
				else
				{
					while (a->blocs[i])
					{
						if(a->blocs[i] == '\'' &&  a->blocs[i+1]&&a->blocs[i+1] == '\'')
							i+=2;
						else if(!a->blocs[i+1])
							break;
						else
							i++;
					}
					if (a->blocs[i] == '\'')
					{
						tmp_str = ft_substr(a->blocs, j, i- j);
						a->acces_args->args[k] = create_arg(tmp_str, 1, 0);
						free(tmp_str);
						cleaned = clean_double_quotes(a->acces_args->args[k]->str);
						free(a->acces_args->args[k]->str);
						a->acces_args->args[k]->str = cleaned;
						if((a->blocs[i+1] == '\'' || a->blocs[i+1] == '"' || a->blocs[i+1]!=' ') && a->acces_args->args[k])
						{
							a->acces_args->args[k]->s = 0;
						}
						k++;
						i++;
					}
				}
				continue ;
			}
			else
			{
				j = i;
				while (a->blocs[i] && a->blocs[i] != ' ' && a->blocs[i] != '\t' && a->blocs[i] != '<'
					&& a->blocs[i] != '>' && a->blocs[i] != '"' && a->blocs[i] != '\'')
				{
					i++;
				}
				if(!a->blocs[i-1] && ((a->blocs[i] == '"' && a->blocs[i+1] == '"') ||(a->blocs[i] == '\'' && a->blocs[i+1] == '\'')))
				{
					i+=2;
					while (a->blocs[i] && a->blocs[i] != ' ' && a->blocs[i] != '\t' && a->blocs[i] != '<'
						&& a->blocs[i] != '>' && a->blocs[i] != '"' && a->blocs[i] != '\'')
						i++;
				}
				if ((!a->blocs[i] || a->blocs[i] == ' ' || a->blocs[i] == '<' || a->blocs[i] == '>' || (a->blocs[i] != '"' && a->blocs[i] != '\'')) && a->blocs[i-1] != '"' && a->blocs[i-1] != '\'')
				{				
					tmp_str = ft_substr(a->blocs, j,i - j);
					a->acces_args->args[k] = create_arg(tmp_str, 2, 0);
					free(tmp_str);
					cleaned = clean_double_quotes(a->acces_args->args[k]->str);
					free(a->acces_args->args[k]->str);
					a->acces_args->args[k]->str = cleaned;
					if(a->blocs[i] == '\'' || a->blocs[i] == '"' || !a->blocs[i])
					{
						a->acces_args->args[k]->s = 0;
					}
					k++;
				}					
				else
				{
					tmp_str = ft_substr(a->blocs, j,i - j);
					a->acces_args->args[k] = create_arg(tmp_str, 2, 0);
					free(tmp_str);
					cleaned = clean_double_quotes(a->acces_args->args[k]->str);
					free(a->acces_args->args[k]->str);
					a->acces_args->args[k]->str = cleaned;
					if(a->blocs[i] == '\'' || a->blocs[i] == '"')
					{
						a->acces_args->args[k]->s = 0;
					}
					k++;
				}
				continue ;
			}
		}
		a->acces_args->args[k] = NULL;
			a = a->next;
	}
}
