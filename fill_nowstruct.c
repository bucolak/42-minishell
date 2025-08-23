/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_nowstruct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:55:03 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/23 13:55:23 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_env_second(t_env *tmp, t_now *get, int j)
{
	int	key_len;
	int	data_len;

	key_len = ft_strlen(tmp->key);
	data_len = ft_strlen(tmp->data);
	get->envp[j] = malloc(key_len + data_len + 2);
	ft_strlcpy(get->envp[j], tmp->key, ft_strlen(tmp->key) + 1);
	get->envp[j][ft_strlen(tmp->key)] = '=';
	ft_strlcpy(get->envp[j] + ft_strlen(tmp->key) + 1, tmp->data,
		ft_strlen(tmp->data) + 1);
}

void	fill_env(t_env *env, t_now *get)
{
	int		j;
	t_env	*tmp;

	j = 0;
	tmp = env;
	while (tmp)
	{
		if (!tmp->key || !tmp->data)
		{
			tmp = tmp->next;
			continue ;
		}
		fill_env_second(tmp, get, j);
		if (!get->envp[j])
			return ;
		tmp = tmp->next;
		j++;
	}
	get->envp[j] = NULL;
}
