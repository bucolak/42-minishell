/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:48:09 by buket             #+#    #+#             */
/*   Updated: 2025/05/09 17:59:52 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_cmd_built_in(t_general *pipe_blocs, t_env **node, char **envp)
{
    int i = 0;
    heredoc(&pipe_blocs);
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
            else if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "unset", 5) == 0)
                unset_cmd(pipe_blocs, node);
            else if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "env", 3) == 0)
                print_env(pipe_blocs ,node, envp, i);
            else if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "exit", 4) == 0)
                exit_cmd(pipe_blocs);
            else if(ft_strncmp(pipe_blocs->acces_args->args[i]->str, "echo", 4) == 0)
                echo_cmd(pipe_blocs);
            i++;
        }
        pipe_blocs = pipe_blocs->next;
    }
}

void echo_cmd(t_general *list)
{
    int i = 1;
    int j = 0;
    int k = 0;
    char *env;
    t_general *tmp;
    tmp = list;
    char *str;
    
    if(!tmp->acces_args->args[1])
        printf("\n");
    else
    {
        while(tmp->acces_args->args[i])
        {
            str = tmp->acces_args->args[i]->str;
            j = 0;
            k = 0;
            if(tmp->acces_args->args[i]->flag == 4)
            {
                printf("burda2\n");
                while(str[j])
                {
                    if(str[j] == '"')
                    {              
                        j++;          
                        while(str[j] && str[j] != '"')
                        {
                            if(str[j] == '$' && str[j+1] != ' ' && str[j+1] != '?')
                            {
                                j++;
                                k = j;
                                while (str[k] && str[k] != ' ' && str[k] != '"' && str[k] != '\'' && str[k] != '$')
                                    k++;
                                char *new = ft_substr(str, j,k-j);
                                env = getenv(new);
                                printf("%s", env);
                                j = k;
                            }
                            else if(str[j] == '$' && str[j+1] == '?')
                            {
                                printf("%d", tmp->dqm);
                                j+=2;
                            }
                            else if(str[j] == '$' && !str[j+1])
                                printf("%c", str[j]);
                            j++;
                        }
                    }
                    if(str[j]!='"' && str[j]!='\'')
                        printf("%c",str[j]);
                    j++;
                }
            }
            else if (tmp->acces_args->args[i]->flag == 1)
            {
                printf("burda333\n");
                env = tmp->acces_args->args[i]->str;
                while(env[j])
                {
                    if(env[j] != '\'')
                        printf("%c", env[j]);
                    j++;
                }
            }
            else
            {
                if(tmp->acces_args->args[i]->flag == 0 || tmp->acces_args->args[i]->flag == 2)
                {
                    printf("burda222\n");
                    while(str[j])
                    {
                        if(str[j] == '$' && str[j+1] != ' ' && str[j+1] != '?' && str[j+1]!='"' && str[j+1])
                        {
                            j++;
                            k = j;
                            while (str[k] && str[k] != ' ' && str[k] != '"' && str[k] != '\'' && str[k] != '$')
                                k++;
                            char *new = ft_substr(str, j,k-j);
                            env = getenv(new);
                            printf("%s", env);
                            j = k; //neden
                        }
                        else if(str[j] == '$' && str[j+1] == '?')
                        {
                            printf("%d", tmp->dqm);
                            j+=2;
                        }
                        if(str[j] != '"')
                            printf("%c", str[j]);
                        j++;
                    }
                    if(tmp->acces_args->args[i+1])
                        printf(" ");
                }
            }
            i++;
        }
        printf("\n");
    }
}

void exit_cmd(t_general *list)
{
    int a;
    if(list->acces_args->args[2])
    {
        printf("exit\n");
        printf("bash: exit: too many arguments\n");
        return ;
    }
    if(list->acces_args->args[1])
    {
        if(is_numeric(list->acces_args->args[1]->str))
        {
            a = ft_atoi(list->acces_args->args[1]->str);
            if(a>=256)
            {
                a-=256;
                while((a - 256)>=0)
                    a-=256;
            }
            list->dqm = a;
            printf("exit\n");
            exit(list->dqm);
        }
        else
        {
            printf("exit\n");
            printf("bash: exit: %s: numeric argument required\n", list->acces_args->args[1]->str);
            list->dqm = 2;
            exit(list->dqm);
        }
    }
    else
    {
        printf("exit\n");
        list->dqm = 0;
        exit(list->dqm);
    }
}

void unset_cmd(t_general *list, t_env **env)
{
    int i = 1;
    t_env *node;
    t_env *pre_node;
    node = *env;
    char *s;
    pre_node =NULL;
    if(ft_strcmp(node->key, list->acces_args->args[i]->str) == 0)
    {
        *env = node->next;
        return ;
    }
    if(list->acces_args->args[i])
    {
        s = list->acces_args->args[i]->str;
        while(node)
        {
            if(ft_strcmp(node->key, s) == 0)
            {
                pre_node->next = node->next;
                free(node->key);
                free(node->data);
                free(node);
                return;
            }
            pre_node = node;
            node = node->next;
        }
    }
}

void print_export_env(t_env **env)
{
    t_env ** new_env;
    t_env *node;
    new_env = export_cmd(env);
    int i = 0;
    int len = ft_lsttsize(*env);
    while(i < len)
    {
        node = new_env[i];
        if(node->key)
        {
            if (node->key && node->data && node->data[0] != '\0')
                printf("declare -x %s\"%s\"\n", node->key, node->data);
            else if(node->key)
                printf("declare -x %s%s\n", node->key, node->data);
        }
        i++;
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