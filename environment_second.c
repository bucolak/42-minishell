/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:19:44 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/24 13:08:54 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_env_2(t_env *node)
{
	if (ft_strcmp(node->key, "=") != 0)
	{
		if (node->data && node->data[0])
			printf("declare -x %s=\"%s\"\n", node->key, node->data);
		else if (node->has_equal == 1)
			printf("declare -x %s=\"\"\n", node->key);
		else
			printf("declare -x %s\n", node->key);
	}
	else
	{
		ft_putstr_fd("bash: export: ", 2);
		ft_putstr_fd("`=': not a valid identifier\n", 2);
	}
}

void	print_export_env(t_env *env, t_general *list)
{
	t_env	**new_env;
	t_env	*node;
	int		i;
	int		len;

	new_env = export_cmd(env);
	i = 0;
	len = ft_lsttsize(env);
	while (i < len)
	{
		node = new_env[i];
		if (node->key)
			print_export_env_2(node);
		i++;
	}
	free(new_env);
	list->dqm = 0;
}

static void	export_cmd_helper_func(t_env *env, t_env **new_env, t_env *swap, int *j)
{
	int	i;
	int	len;

	len = ft_lsttsize(env);
	i = 0;
	while (i < len - 1)
	{
		*j = 0;
		while (*j < len - 1)
		{
			if (ft_strcmp(new_env[*j]->key, new_env[*j + 1]->key) > 0)
			{
				swap = new_env[*j];
				new_env[*j] = new_env[*j + 1];
				new_env[*j + 1] = swap;
			}
			(*j)++;
		}
		i++;
	}
}

t_env	**export_cmd(t_env *env)
{
	int i;
	int j;
	int len;
	t_env **new_env;
	t_env *tmp;
	t_env *swap;

	swap = NULL;
	len = ft_lsttsize(env);
	i = 0;
	new_env = malloc(sizeof(t_env *) * len);
	tmp = env;
	while (i < len)
	{
		new_env[i] = tmp;
		tmp = tmp->next;
		i++;
	}
	export_cmd_helper_func(env, new_env, swap, &j);
	return (new_env);
}
