/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:48:09 by buket             #+#    #+#             */
/*   Updated: 2025/05/01 17:48:38 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_cmd_built_in(t_general *pipe_blocs, t_env **node, char **envp)
{
    int i = 0;
    while(pipe_blocs)
    {
        i = 0;
        while(pipe_blocs->acces_args->args[i])
        {
            if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "cd", 2) == 0)
                cd_cmd(pipe_blocs->acces_args->args);
            else if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "pwd", 3) == 0)
                pwd_cmd(&pipe_blocs->acces_args->args[i]->str);
            else if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "export", 6) == 0)
            {
                if((pipe_blocs->acces_args->args[i+1] && pipe_blocs->acces_args->args[i+1]->str) && !pipe_blocs->acces_args->args[i+2])
                    create_env(pipe_blocs, node);
                else
                    print_export_env(node);
            }
            else if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "env", 3) == 0)
                print_env(pipe_blocs ,node, envp, i);
            i++;
        }
        pipe_blocs = pipe_blocs->next;
    }
}



void print_export_env(t_env **env)
{
    t_env ** new_env;
    t_env *tmp;
    t_env *node;
    new_env = export_cmd(env);
    int i = 0;
    int len = ft_lsttsize(*env);
    while(i < len)
    {
        node = new_env[i];
        if(node->key)
            printf("declare -x %s\"%s\"\n", node->key, node->data);
        i++;
    }
    tmp = *env;
    while(tmp)
    {
        if (tmp->key && tmp->data && tmp->data[0] != '\0')
                printf("declare -x %s\"%s\"\n", tmp->key, tmp->data);
        else if(tmp->key)
            printf("declare -x %s%s\n", tmp->key, tmp->data);
        tmp = tmp->next;
    }
}

t_env **export_cmd(t_env **env)
{
    int i;
    int j;
    int len;
    t_env **new_env;
    t_env *tmp;
    t_env *swap;
    len = ft_lsttsize(*env);
    i = 0;
    new_env = env;
    new_env =malloc(sizeof(t_env *) * len);
    tmp = *env;
    while(i < len)
    {
        new_env[i] = tmp;
        tmp = tmp->next;
        i++;
    }
    i = 0;
    while(i < len-1)
    {
        j = 0;
        while(j < len-1)
        {
            if(ft_strcmp(new_env[j]->key, new_env[j+1]->key) > 0)
            {
                swap = new_env[j];
                new_env[j] = new_env[j + 1];
                new_env[j + 1] = swap;
            }
            j++;
        }
        i++;
    }
    return new_env;
}

void cd_cmd(t_arg **args)
{
    if (!args[1])
    {
        char *line;
        line = getenv("HOME");
        if (line)
            chdir(line);
    }
    else
        chdir(args[1]->str);
}

void pwd_cmd(char **ar)
{
    char *line;
    if(ft_strncmp(ar[0],"pwd",3)==0)
    {
        line = getcwd(NULL, 0);
        if(!line)
        {
            printf("Error\n");
            free(line);
            //free_split(ar);
            exit(1);
        }
        printf("%s\n", line);
    }
}