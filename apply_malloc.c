/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:29:26 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/13 18:10:45 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_arg *create_arg(const char *str, int flag)
// {
//     t_arg *arg = malloc(sizeof(t_arg));
//     if (!arg)
//         return NULL;
//     arg->str = ft_strdup(str);
//     arg->flag = flag;
//     return arg;
// }
t_arg *create_arg(char *str, int flag, int type)
{
    char *new;
    int i = 0;
    int j = 0;
    t_arg *arg = malloc(sizeof(t_arg));
    if (!arg)
        return NULL;
    new =ft_calloc((ft_strlen(str)+1), sizeof(char));
    if(type == 4)
    {
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
        arg->str = ft_strdup(new);
        free(new);
        arg->flag = flag;
        return arg;
    }
    arg->str = ft_strdup(str);
    arg->flag = flag;
    free(new);
    free(str);
    return arg;
}
t_pipeafter *create_pipeafter(void)
{
    t_pipeafter *pa = malloc(sizeof(t_pipeafter));
    if (!pa)
    {
        free(pa);
        return NULL;
    }
    pa->args = ft_calloc(100, sizeof(t_arg *)); // max arg 100 (geliştirilebilir)
    if (!pa->args)
    {
        free(pa->args);
        free(pa);
        return NULL;
    }
    return pa;
}

t_general *create_general_node(int dqm)
{
    static int last_dqm = 0;
    t_general *node = malloc(sizeof(t_general));
    if (!node)
    {
       free(node); 
        return NULL;
    }
    last_dqm = dqm;
    node->dqm = last_dqm;
    node->acces_args = create_pipeafter();
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
