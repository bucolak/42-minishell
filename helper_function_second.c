/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function_second.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:50:13 by seerel            #+#    #+#             */
/*   Updated: 2025/08/24 16:45:45 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	connect_count_malloc(t_general *list)
{
	t_general	*tmp;
	int			i;
	int			c;
	int			j;
	char		*new;

	tmp = list;
	i = 0;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			if (tmp->acces_args->args[i]->s == 0 && tmp->acces_args->args[i + 1])
			{
				if (is_redirection(tmp->acces_args->args[i]->str) == 1
					&& tmp->acces_args->args[i]->flag != 0
					&& tmp->acces_args->args[i]->flag != 1
					&& is_redirection(tmp->acces_args->args[i + 1]->str) == 0
					&& (tmp->acces_args->args[i + 1]->flag == 0
						|| tmp->acces_args->args[i + 1]->flag == 1))
				{
					i++;
					continue ;
				}
				c = ft_strlen(tmp->acces_args->args[i]->str)
					+ ft_strlen(tmp->acces_args->args[i + 1]->str);
				new = malloc(sizeof(char) * (c + 1));
				ft_strlcpy(new, tmp->acces_args->args[i]->str, c + 1);
				ft_strlcat(new, tmp->acces_args->args[i + 1]->str, c + 1);
				if ((ft_strcmp(new, "<<") == 0 || ft_strcmp(new, ">>") == 0)
					&& ((tmp->acces_args->args[i]->flag != 5
							&& tmp->acces_args->args[i]->flag != 2)
						|| (tmp->acces_args->args[i + 1]->flag != 5
							&& tmp->acces_args->args[i + 1]->flag != 2)))
				{
					tmp->acces_args->args[i]->flag = 0;
				}
				if (tmp->acces_args->args[i + 1]
					&& is_redirection(tmp->acces_args->args[i + 1]->str) == 1
					&& tmp->acces_args->args[i + 1]->flag != 5
					&& tmp->acces_args->args[i + 1]->s == 0)
				{
					tmp->acces_args->args[i]->flag = 6;
				}
				tmp->acces_args->args[i]->s = tmp->acces_args->args[i + 1]->s;
				free(tmp->acces_args->args[i + 1]->str);
				free(tmp->acces_args->args[i + 1]);
				free(tmp->acces_args->args[i]->str);
				tmp->acces_args->args[i]->str = new;
				j = i + 1;
				while (tmp->acces_args->args[j])
				{
					tmp->acces_args->args[j] = tmp->acces_args->args[j + 1];
					j++;
				}
				tmp->acces_args->args[j] = NULL;
				continue ;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

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

void	remove_null(t_general *list)
{
	t_general	*tmp;
	char		*str;
	int			i;
	int			j;
	int			k;

	tmp = list;
	char *new; //    echo "" "" hi
	i = 0;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			j = 0;
			k = 0;
			str = tmp->acces_args->args[i]->str;
			new = malloc(sizeof(char) * count_remove_null(str));
			if (!new)
				return ;
			while (tmp->acces_args->args[i]->str[j])
			{
				if (((str[j] == '"' && str[j + 1] && str[j + 1] == '"'
							&& ((j > 0 && str[j - 1]) || str[j + 2])))
					|| ((str[j] == '\'' && str[j + 1] && str[j + 1] == '\''
							&& ((j > 0 && str[j - 1]) || str[j + 2]))))
				{
					j += 2;
				}
				else
				{
					new[k++] = str[j++];
				}
			}
			new[k] = '\0';
			if (tmp->acces_args->args[i]->str)
				free(tmp->acces_args->args[i]->str);
			tmp->acces_args->args[i]->str = new;
			i++;
		}
		tmp = tmp->next;
	}
}

void	control_redireciton(t_general *list, t_env *env)
{
	t_general	*tmp;
	char		*str;
	t_env		*tenv;
	int			i;
	int			j;

	str = NULL;
	tmp = list;
	while (tmp)
	{
		i = 0;
		while (tmp->acces_args->args[i])
		{
			if (tmp->acces_args->args[i]->flag == 6)
			{
				j = 0;
				while (tmp->acces_args->args[i]->str[j])
				{
					if (tmp->acces_args->args[i]->str[j] == '='
						&& tmp->acces_args->args[i]->str[j + 1])
					{
						j++;
						str = tmp->acces_args->args[i]->str + j;
						break ;
					}
					j++;
				}
				tenv = env;
				while (tenv)
				{
					if (ft_strncmp(str, tenv->key, ft_strlen(tenv->key)) == 0)
					{
						tenv->f = 3;
						break ;
					}
					tenv = tenv->next;
				}
			}
			i++;
		}
		tmp = tmp->next;
	}
}
