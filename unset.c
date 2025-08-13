/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:45:34 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/13 20:58:38 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void remove_env_var(t_env **env, char *var_name)
{
    t_env *current = *env;
    t_env *prev = NULL;
    size_t name_len = ft_strlen(var_name);
    
    if (!env || !*env || !var_name)
        return;
    
    while (current)
    {
        if (ft_strcmp(current->key, var_name) == 0
            && (current->key[name_len] == '=' || current->key[name_len] == '\0'))
        {
            if (prev == NULL)
                *env = current->next;
            else
                prev->next = current->next;            
            if (current->key)
            {
                free(current->key);
                current->key = NULL;
            }
            if (current->data)
            {
                free(current->data);
                current->data = NULL;
            }
            free(current);
            current = NULL;
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
