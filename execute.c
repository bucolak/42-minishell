/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/05/10 15:30:20 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_output(char ***ar)
{
    int i = 0;
    int fd;
    struct stat path_stat;
    while ((*ar)[i])
    {
        if (ft_strcmp((*ar)[i], ">") == 0)
        {
            if ((*ar)[i + 1])
            {
                i++;
                if (stat((*ar)[i], &path_stat) != 0)
                    fd = open((*ar)[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                else
                    fd = open((*ar)[i], O_WRONLY | O_TRUNC, 0644);
                if (fd < 0)
                    perror("open");
                dup2(fd, 1);
                close(fd);
                renew_block(ar);
                break;
            }
        }
        i++;
    }
}

void execute_command(char **ar, char **envp)
{
    int i;
    i = 0;
        char *args = getenv("PATH");
    char **paths = ft_split(args, ':');
        while(paths[i])
        {
            char *str = ft_strjoin(paths[i], "/");
            char *end = ft_strjoin(str, ar[0]);
            if(access(end, X_OK) == 0)
            {
                execve(end, ar, envp);
                free(str);
                free(end);
                return ;
            }
            i++;
        }
}

void check_cmd_sys_call(t_general *pipe_blocs, t_env **env)
{
    pid_t pid;
    t_env *tmp;
    char **ar;
    char **envp;
    int j;
    
    pid = fork();
    ar = malloc(sizeof(char **));
    tmp = *env;
    envp = malloc(sizeof(char *) * ft_lsttsize(*env));
    j = 0;
    while(pipe_blocs->acces_args->args[j])
    {
        ar[j] = ft_strdup(pipe_blocs->acces_args->args[j]->str);
        j++;
    }
    ar[j] = NULL;
    j = 0;
    while(tmp)
    {
        size_t len = strlen(tmp->key) + strlen(tmp->data) + 1;
        envp[j] = malloc(len);
        ft_strlcpy(envp[j],tmp->key, ft_strlen(tmp->key));
        ft_strlcpy(envp[j] + ft_strlen(tmp->key), tmp->data,ft_strlen(tmp->data));
        if (!envp[j])
            return ;
        tmp = tmp->next;
        j++;
    }
    envp[j] = NULL;
    if(pid == 0)
    {
        handle_output(&ar);
        execute_command(ar, envp);
    }
    else
        waitpid(pid,NULL, 0);
}
