/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_first.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:58:25 by buket             #+#    #+#             */
/*   Updated: 2025/08/24 13:10:18 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_env_helper_func(int *i, int *j, t_env *tmp, char **envp)
{
	if (tmp->key)
		free(tmp->key);
	if (tmp->data)
		free(tmp->data);
	if (envp[*j][*i] == '=')
	{
		tmp->key = ft_substr(envp[*j], 0, *i);
		tmp->data = ft_strdup(envp[*j] + *i + 1);
		tmp->has_equal = 1;
	}
	else
	{
		tmp->key = ft_substr(envp[*j], 0, *i);
		tmp->data = NULL;
		tmp->has_equal = 0;
	}
}

void	get_env(t_env *node, char **envp)
{
	static int	j;
	int			i;
	t_env		*tmp;

	j = 0;
	i = 0;
	tmp = node;
	while (envp[j])
	{
		i = 0;
		while (envp[j][i] && envp[j][i] != '=')
		{
			i++;
		}
		get_env_helper_func(&i, &j, tmp, envp);
		j++;
		if (envp[j])
		{
			tmp->next = create_env_node();
			tmp = tmp->next;
		}
	}
}

static void	print_env_scnd(t_env *tmp)
{
	if (tmp->data && tmp->data[0])
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(tmp->data, 1);
		ft_putchar_fd('\n', 1);
	}
	else if (tmp->has_equal == 1)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putstr_fd("=\n", 1);
	}
}

void	print_env(t_general *list, t_env *node, int i)
{
	t_env	*tmp;

	if (list->acces_args->args[i + 1])
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(list->acces_args->args[i + 1]->str, 2);
		ft_putstr_fd("’: No such file or directory\n", 2);
		list->dqm = 127;
		return ;
	}
	tmp = node;
	while (tmp)
	{
		if (tmp->key && tmp->data)
			print_env_scnd(tmp);
		tmp = tmp->next;
	}
	list->dqm = 0;
}

int	key_cont(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!(key[i] >= 65 && key[i] <= 90) && !(key[i] >= 97 && key[i] <= 122)
			&& key[i] != 61 && key[i] != 95)
			return (0);
		i++;
	}
	return (1);
}
