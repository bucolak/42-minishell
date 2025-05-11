/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/05/11 15:27:57 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_append(t_now *get, t_general *list)
{
    int i = 0;
    int fd;
    struct stat path_stat;
    while (get->ar[i])
    {
        if (ft_strcmp(get->ar[i], ">>") == 0)
        {
            if (get->ar[i + 1])
            {
                i++;
                if (stat(get->ar[i], &path_stat) != 0)
                    fd = open(get->ar[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
                else
                    fd = open(get->ar[i], O_WRONLY | O_APPEND, 0644);
                if (fd < 0)
                    perror("open");
                dup2(fd, 1);
                close(fd);
                renew_block(&get->ar);

                renew_block2(list);
                break;
            }
        }
        i++;
    }
}

void handle_input(t_now *get)
{
    int i = 0;
    int fd;
    struct stat path_stat;
    while (get->ar[i])
    {
        if (ft_strcmp(get->ar[i], "<") == 0)
        {
            if (get->ar[i + 1])
            {
                i++;
                if (stat(get->ar[i], &path_stat) != 0)
                    fd = open(get->ar[i], O_CREAT | O_RDONLY, 0644);
                else
                    fd = open(get->ar[i], O_RDONLY, 0644);
                if (fd < 0)
                    perror("open");
                dup2(fd, 0);
                close(fd);
                renew_block(&get->ar);
                break;
            }
        }
        i++;
    }
}

void handle_output(t_now *get)
{
    int i = 0;
    int fd;
    struct stat path_stat;
    while (get->ar[i])
    {
        if (ft_strcmp(get->ar[i], ">") == 0)
        {
            if (get->ar[i + 1])
            {
                i++;
                if (stat(get->ar[i], &path_stat) != 0)
                    fd = open(get->ar[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                else
                    fd = open(get->ar[i], O_WRONLY | O_TRUNC, 0644);
                if (fd < 0)
                    perror("open");
                dup2(fd, 1);
                close(fd);
                renew_block(&get->ar);
                break;
            }
        }
        i++;
    }
}

void execute_command(t_now *get)
{
    int i;
    i = 0;
        char *args = getenv("PATH");
    char **paths = ft_split(args, ':');
        while(paths[i])
        {
            char *str = ft_strjoin(paths[i], "/");
            char *end = ft_strjoin(str, get->ar[0]);
            if(access(end, X_OK) == 0)
            {
                execve(end, get->ar, get->envp);
                free(str);
                free(end);
                return ;
            }
            i++;
        }
}

void fill_env(t_env **env, t_now *get)
{
    int j;
    j = 0;
     t_env *tmp;
     tmp = *env;
    while(tmp)
    {
        get->envp[j] = malloc(ft_strlen(tmp->key) + ft_strlen(tmp->data)+ 2);
        ft_strlcpy(get->envp[j],tmp->key, ft_strlen(tmp->key)+1);
        ft_strlcpy(get->envp[j] + ft_strlen(tmp->key), tmp->data,ft_strlen(tmp->data)+1);
        if (!get->envp[j])
            return ;
        tmp = tmp->next;
        j++;
    }
    get->envp[j] = NULL;
}

void fill_ar(t_general *pipe_blocs, t_now *get)
{
    int j;
     j = 0;
    while(pipe_blocs->acces_args->args[j])
    {
        get->ar[j] = ft_strdup(pipe_blocs->acces_args->args[j]->str);
        j++;
    }
    get->ar[j] = NULL;
}

void check_cmd_sys_call(t_general *pipe_blocs, t_env **env)
{
    pid_t pid;
    t_now *get;
    int arg_count;
    get = malloc(sizeof(t_now));
    pid = fork();
    arg_count = 0;
    while(pipe_blocs->acces_args->args[arg_count])
        arg_count++;
    get->ar = malloc(sizeof(char *) * (arg_count + 1));
    get->envp = malloc(sizeof(char *) * ft_lsttsize(*env));
    fill_ar(pipe_blocs, get);
    fill_env(env, get);
    if (pid == 0)
    {
        handle_output(get);
        handle_input(get);
        handle_append(get, pipe_blocs);

        if(ft_strcmp(get->ar[0], "echo") == 0)
        {
            initalized_echo(pipe_blocs);
            exit(0) ;
        }
        else
            execute_command(get);
        exit(0);
    }
    else
        waitpid(pid,NULL, 0);
}
