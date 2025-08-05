/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:29:26 by bucolak           #+#    #+#             */
/*   Updated: 2025/08/05 14:26:18 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void complete_create_arg(char *str, char *new)
{
    int i;
    int j;
    
    i = 0;
    j = 0;
    while(str[j])
    {
        if(str[j] != '"' && str[j] != '\'')
        {
            new[i] = str[j];
            i++;
        }
        j++;
    }
    new[i] = '\0';
}

t_arg *create_arg(char *str, int flag, int type)
{
    //char *new;
    (void)type;
    t_arg *arg = malloc(sizeof(t_arg));
    if (!arg)
        return NULL;
    // if(type == 4)
    // {
    //     new =ft_calloc((ft_strlen(str)+1), sizeof(char));
    //     if (!new)
    //     {
    //         free(arg);
    //         return NULL;
    //     }
    //     complete_create_arg(str, new);
    //     arg->str = new;
    //     arg->s = 1;
    //     arg->flag = flag;
    //     return arg;
    // }
    arg->str = ft_strdup(str);
    if (!arg->str)
    {
        free(arg);
        return NULL;
    }
    arg->flag = flag;
    arg->s = 1;
    return arg;
}

t_pipeafter *create_pipeafter(void)
{
    t_pipeafter *pa = malloc(sizeof(t_pipeafter));
    if (!pa)
    {
        return NULL;
    }
    pa->args = NULL;
    return pa;
}

t_general *create_general_node(int dqm)
{
    static int last_dqm = 0;
    t_general *node = malloc(sizeof(t_general));
    if (!node)
    {
        return NULL;
    }
    last_dqm = dqm;
    node->dqm = last_dqm;
    node->acces_args = create_pipeafter();
    if (!node->acces_args)
    {
        free(node);
        return NULL;
    }
    node->acces_args->args=NULL;
    node->heredoc_fd = -1;
    node->blocs = NULL;
    node->limiter = NULL;
    node->next = NULL;
    return node;
}

t_env *create_env_node(void)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    node->key = NULL;
    node->data = NULL;
    node->next = NULL;
    return node;
}
