/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:58:25 by buket             #+#    #+#             */
/*   Updated: 2025/05/02 16:11:14 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void get_env(t_env **node, char **envp)
{
    static int j = 0;
    int k;
    int i = 0;
    t_env *tmp;
    tmp = *node;
    while(envp[j])
    {
        i = 0;
        k = 0;
        while(envp[j][i]!='=')
            i++;    
        if(envp[j][i]=='=')
        {
            tmp->key = ft_substr(envp[j], k, i-k+1);
            tmp->data =ft_strdup(envp[j] + i + 1);
        }
        else
        {
            tmp->key = ft_substr(envp[j], k, i-k);
        }
        j++;
        if (envp[j]) 
        {
            tmp->next = create_env_node();
            tmp = tmp->next;
        }
    }
}

void print_env(t_general *list, t_env **node, char **envp, int i)
{
    t_env *tmp;
    
    if (list->acces_args->args[i+1])
    {
        printf("Error\n");
        return;
    }
    tmp = *node;
    while(tmp)
    {
        if (tmp->key && tmp->data)
        {
            if(ft_strchr(tmp->key, '='))
                printf("%s%s\n", tmp->key, tmp->data);
        }
        tmp = tmp->next;
    }
}

void	ft_envadd_back(t_env **lst, char *key, char *data)
{
	t_env	*last;
    t_env *new_node;
    new_node = create_env_node();
    if(key)
    {
            new_node->key = ft_strdup(key);
            new_node->data = ft_strdup(data);
	    if (*lst == NULL)
            *lst = new_node;
	    else
	    {
            last = *lst;
            while(last->next)
                last = last->next;
            last->next = new_node;
	    }
    }
}

char *get_key(char *str)
{
    int i = 0;
    int start = 0;
    char *key;
    while(str[i])
    {
        if(str[i] ==' ')
        {
            printf("error\n");
            return NULL;
        }
        i++;
    }
    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    int end = i;
    if (str[i] == '=')
        end = i + 1;
    key = ft_substr(str, start, end - start);
    return ft_strtrim(key,"'\"");
}

char *get_data(char *str)
{
    int i = 0;
    int k = 0;
        
    while(str[i] && str[i] != '=')
        i++;
    if(str[i] == '=')
        i++;
    if((str[i] =='"' || str[i] =='\''))
        i++;
    k = i;
    while(str[i] && str[i])
        i++;
    if(str[i-1] =='"' || str[i] =='\'')
        i--;
    return ft_substr(str, k, i-k);
}

void create_env(t_general *list, t_env **env)
{
    int i;
    int j;
    while(list)
    {
        i = 0;
        while(list->acces_args->args[i])
        {
            if(ft_strncmp(list->acces_args->args[i]->str, "export", 6) == 0)
            {
                i++;
                char *new = list->acces_args->args[i]->str;
                if(list->acces_args->args[i] && new)
                {
                    if((count_dquote(new) %2==0 || count_squote(new)%2==0) 
                    && is_repeated(env, get_key(new), get_data(new)) == 0)
                        ft_envadd_back(env, get_key(new), get_data(new));
                }
                    
            }
            i++;
        }
        list = list->next;
    }
}
