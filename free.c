/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:12:21 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/02 19:37:05 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_split(char **str)
{
    int i;
    if(!str)
        return ;
    i = 0;
    while(str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void	free_envp(char **envp)
{
	int i = 0;
	if (!envp)
		return;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void free_env(t_env *env)
{
    t_env *tmp;
    tmp = env;
    while(tmp)
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

void free_pipe_blocks(t_general *blocks)
{
    t_general *tmp;
    int i;
    tmp = blocks;
    while(tmp)
    {
        i = 0;
        while(tmp->acces_args->args[i])
        {
            if(tmp->acces_args->args[i]->str)
                free(tmp->acces_args->args[i]->str);
            free(tmp->acces_args->args[i]);
            i++;
        }
        free(tmp->acces_args->args);
        free(tmp->acces_args);
        tmp = tmp->next;
    }
}