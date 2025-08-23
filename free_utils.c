/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:52:53 by seerel            #+#    #+#             */
/*   Updated: 2025/08/23 20:04:52 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_limiter(char **limiter)
{
	int	i;

	if (!limiter)
		return ;
	i = 0;
	while (limiter[i])
	{
		free(limiter[i]);
		i++;
	}
	free(limiter);
}

void	free_acces_args(t_pipeafter *acc)
{
	int	i;

	if (!acc)
		return ;
	if (acc->args)
	{
		i = 0;
		while (acc->args[i])
		{
			if (acc->args[i]->str)
				free(acc->args[i]->str);
			free(acc->args[i]);
			acc->args[i] = NULL;
			i++;
		}
		free(acc->args);
	}
	free(acc);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->data)
			free(env->data);
		free(env);
		env = tmp;
	}
}

void	free_envp(t_now *get)
{
	int	i;

	if (!get)
		return ;
	i = 0;
	while (get->envp[i])
	{
		free(get->envp[i]);
		i++;
	}
	free(get->envp);
	free(get);
}