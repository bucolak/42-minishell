/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:29:26 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/23 17:57:47 by bucolak          ###   ########.fr       */
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
t_arg *create_arg(const char *str, int flag, int type)
{
    char *new;
    int i = 0;
    int j = 0;
    t_arg *arg = malloc(sizeof(t_arg));
    if (!arg)
        return NULL;
    new = malloc(sizeof(char) * ft_strlen(str));
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
        arg->str = ft_strdup(new);
        arg->flag = flag;
        return arg;
    }
    arg->str = ft_strdup(str);
    arg->flag = flag;
    return arg;
}
t_pipeafter *create_pipeafter(void)
{
    t_pipeafter *pa = malloc(sizeof(t_pipeafter));
    if (!pa)
        return NULL;
    pa->args = malloc(sizeof(t_arg *) * 100); // max arg 100 (geliştirilebilir)
    if (!pa->args)
    {
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
        return NULL;
    last_dqm = dqm;
    node->dqm = last_dqm;
    node->acces_args = create_pipeafter();
    node->blocs = NULL;
    node->next = NULL;
    return node;
}

t_env *create_env_node(void)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    node->data = NULL;
    node->next = NULL;
    return node;
}
