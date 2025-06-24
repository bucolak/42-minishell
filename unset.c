/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:45:34 by bucolak           #+#    #+#             */
/*   Updated: 2025/06/23 23:08:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void remove_env_var(t_env **env, char *var_name)
{
	t_env *current = *env;
	t_env *prev = NULL;
	size_t name_len = ft_strlen(var_name);

	while (current)
	{
		if (ft_strncmp(current->key, var_name, name_len) == 0 
			&& (current->key[name_len] == '=' || current->key[name_len] == '\0'))
		{
			if (prev == NULL)
				*env = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->data);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

void unset_cmd(t_general *list, t_env **env)
{
	int i = 1;

	while (list->acces_args->args[i])
	{
		if (list->acces_args->args[i]->str)
		{
			remove_env_var(env, list->acces_args->args[i]->str);
		}
		i++;
	}
	list->dqm = 0;
}
