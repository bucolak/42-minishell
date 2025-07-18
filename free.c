/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:12:21 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/18 23:13:37 by buket            ###   ########.fr       */
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
        i = 0;
        if (tmp->acces_args && tmp->acces_args->args)
        {
            while (tmp->acces_args->args[i])
            {
                if (tmp->acces_args->args[i]->str)
                {
                    free(tmp->acces_args->args[i]->str);
                    tmp->acces_args->args[i]->str = NULL;
                }
                free(tmp->acces_args->args[i]);
                i++;
            }
            free(tmp->acces_args->args);
            if (tmp->acces_args)
                free(tmp->acces_args);
        }
        i = 0;
        if (tmp->limiter)
        {
            while(tmp->limiter[i])
            {
                free(tmp->limiter[i]);
                i++;
            }
        }
        free(tmp->limiter);
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
    int i;
    i = 0;
    while (get->envp[i])
    {
        free(get->envp[i]);
        i++;
    }
    free(get->envp);
    free(get);
}