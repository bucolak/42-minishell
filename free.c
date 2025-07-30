/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:12:21 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/30 18:53:21 by buket            ###   ########.fr       */
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
    t_general *next;
    int i;

    tmp = blocks;
    while (tmp)
    {
        // Free acces_args and its args
        if (tmp->acces_args)
        {
            if (tmp->acces_args->args)
            {
                i = 0;
                while (tmp->acces_args->args[i])
                {
                    if (tmp->acces_args->args[i]->str)
                        free(tmp->acces_args->args[i]->str);
                    free(tmp->acces_args->args[i]);
                    tmp->acces_args->args[i] = NULL;
                    i++;
                }
                free(tmp->acces_args->args);
            }
            free(tmp->acces_args);
        }
        // Free limiter array
        if (tmp->limiter)
        {
            i = 0;
            while(tmp->limiter[i])
            {
                free(tmp->limiter[i]);
                i++;
            }
            free(tmp->limiter);
        }
        // Free blocs string
        if (tmp->blocs)
            free(tmp->blocs);
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
}

void free_pipe(t_pipe *pipe)
{
    int i = 0;
    while(i < pipe->count-1)
    {
        free(pipe->fd[i]);
        i++;
    }   
    free(pipe->fd);
    free(pipe->pid);
    free(pipe);
    pipe = NULL;
}

void free_envp(t_now *get)
{
    if (!get)
        return;
    int i = 0;
    while (get->envp[i])
    {
        free(get->envp[i]);
        i++;
    }
    free(get->envp);
    free(get);
}