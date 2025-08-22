/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:12:21 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/22 19:24:53 by bucolak          ###   ########.fr       */
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
    
    while (env)
    {
        tmp = env->next;
        if (env->key)
        {
            free(env->key);
            env->key = NULL;
        }
        if (env->data)
        {
            free(env->data);
            env->data = NULL;
        }
        free(env);
        env = tmp;
    }
}

void free_pipe_blocks(t_general *blocks)
{
    t_general *tmp;
    t_general *next;
    int i;
    if (!blocks) return;
    tmp = blocks;
    while (tmp)
    {
        if (tmp->acces_args)
        {
            if (tmp->acces_args->args)
            {
                i = 0;
                while (tmp->acces_args->args[i])
                {
                    if (tmp->acces_args->args[i]->str)
                    free(tmp->acces_args->args[i]->str);
                    if(tmp->acces_args->args[i])
                    free(tmp->acces_args->args[i]);
                    tmp->acces_args->args[i] = NULL;
                    i++;
                }
                if(tmp->acces_args->args)
                free(tmp->acces_args->args);
            }
            if(tmp->acces_args)
            free(tmp->acces_args);
        }
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

void	close_all_open_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void cleanup(t_full *full)
{
    close_all_open_fds();
    if(full->get)
    {
        free_envp(full->get);
        full->get = NULL;
    }
    if(full->pipe_blocks->next && full->pipe)
    {
        free_pipe(full->pipe);
        full->pipe=NULL;
    }
    if(full->new)
    {
        free_split(full->new);
        full->new = NULL;
    }
    if(full->node)
    {
        free_env(full->node);
        full->node=NULL;
    }
    close_heredoc_fd(full->pipe_blocks);
}