/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:47:15 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 20:11:20 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_envadd_back_scnd(t_env **lst, char *key, char *data,
		t_env *new_node)
{
	t_env	*last;

	new_node->key = ft_strdup(key);
	if (data)
		new_node->data = ft_strdup(data);
	else
		new_node->data = ft_strdup("");
	if (*lst == NULL)
		*lst = new_node;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
}

static int	repeat_control_ft_envadd_back(t_env *tmp, char *key, char *data,
		t_full *full)
{
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (tmp->data)
				free(tmp->data);
			tmp->data = ft_strdup(data);
			full->pipe_blocks->dqm = 0;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_envadd_back(t_env **lst, char *key, char *data, t_full *full)
{
	t_env	*new_node;
	t_env	*tmp;

	tmp = *lst;
	if (repeat_control_ft_envadd_back(tmp, key, data, full) == 1)
		return ;
	new_node = create_env_node();
	if (key)
	{
		if (key_cont(key) == 1)
		{
			ft_envadd_back_scnd(lst, key, data, new_node);
			full->pipe_blocks->dqm = 0;
		}
	}
}

static void	create_env_scnd(t_general *list, t_env **env, int i, t_full *full)
{
	char	*new;
	char	*key;
	char	*data;

	new = list->acces_args->args[i]->str;
	if (list->acces_args->args[i] && new)
	{
		list->dqm = 0;
		key = get_key(new);
		data = get_data(new);
		if ((count_dquote(new) % 2 == 0 || count_squote(new) % 2 == 0))
		{
			if (key && key[0])
				ft_envadd_back(env, key, data, full);
			else
			{
				ft_putstr_fd("bash: export: `", 2);
				ft_putstr_fd(list->acces_args->args[1]->str, 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				list->dqm = 1;
			}
		}
		free(key);
		free(data);
	}
}

void	create_env(t_general *list, t_env **env, t_full *full)
{
	int	i;

	while (list)
	{
		i = 0;
		while (list->acces_args->args[i])
		{
			if (ft_strncmp(list->acces_args->args[i]->str, "export", 6) == 0)
			{
				i++;
				while (list->acces_args->args[i])
				{
					create_env_scnd(list, env, i, full);
					i++;
				}
				break ;
			}
			i++;
		}
		list = list->next;
	}
}
