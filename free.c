/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:12:21 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/07 00:37:25 by buket            ###   ########.fr       */
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
    free(tmp);
}

void free_pipe_blocks(t_general *blocks)
{
    t_general *new_tmp;
    int i;
    new_tmp = blocks;
    while(new_tmp)
    {
        i = 0;
        new_tmp = blocks->next;
        while(blocks->acces_args->args[i])
        {
            if(blocks->acces_args->args[i]->str)
                free(blocks->acces_args->args[i]->str);
            free(blocks->acces_args->args[i]);
            i++;
        }
        free(blocks->acces_args->args);
        free(blocks->acces_args);
        free(blocks);
        blocks = new_tmp;
    }
}
