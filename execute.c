/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/05/11 21:19:56 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_heredoc(t_general *list) // burada kaldım
{
    int i = 0;
    char *line;
    t_heredoc *heredoc;
    t_heredoc *new_node;
    t_heredoc *tmp;
    heredoc = NULL;
    new_node = NULL;
    while (list->acces_args->args[i])
    {
        if (ft_strcmp(list->acces_args->args[i]->str, "<<") == 0 && list->acces_args->args[i+1])
        {
            i++;
            while(1)
            {
                new_node = malloc(sizeof(t_heredoc));
                line = readline("> ");
                new_node->content = ft_strdup(line);
                new_node->next = NULL;
                if(ft_strcmp(line, list->acces_args->args[i]->str) == 0)
                {
                    break;
                }
                if(!heredoc)
                {
                    heredoc = new_node;
                    tmp = heredoc;
                }
                else
                {
                    heredoc->next = new_node;
                    tmp = new_node;
                }
            }
            while (heredoc)
            {
                ft_putstr_fd(heredoc->content, 1);
                ft_putchar_fd('\n', 1);
                heredoc = heredoc->next;
            }
        }
        i++;
    }
}


void handle_append(t_general *list)
{
    int i = 0;
    int fd;
    while (list->acces_args->args[i])
    {
        if (ft_strcmp(list->acces_args->args[i]->str, ">>") == 0)
        {
            if (list->acces_args->args[i+1])
            {
                i++;
                fd = open(list->acces_args->args[i]->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
                if (fd < 0)
                    perror("open");
                dup2(fd, 1);
                close(fd);
                renew_block2(list);
                break;
            }
        }
        i++;
    }
}

void handle_input(t_general *list)
{
    int i = 0;
    int fd;
    while (list->acces_args->args[i])
    {
        if (ft_strcmp(list->acces_args->args[i]->str, "<") == 0)
        {
            if (list->acces_args->args[i+1])
            {
                i++;
                fd = open(list->acces_args->args[i]->str, O_CREAT | O_RDONLY, 0644);
                if (fd < 0)
                    perror("open");
                dup2(fd, 0);
                close(fd);
                break;
            }
        }
        i++;
    }
}

void handle_output(t_general *list)
{
    int i = 0;
    int fd;
    while (list->acces_args->args[i])
    {
        if (ft_strcmp(list->acces_args->args[i]->str, ">") == 0)
        {
            if (list->acces_args->args[i+1])
            {
                i++;
                fd = open(list->acces_args->args[i]->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            
                if (fd < 0)
                    perror("open");
                dup2(fd, 1);
                close(fd);
                break;
            }
        }
        i++;
    }
}

void execute_command(t_general *pipe_blocs, t_now *get)
{
    int i;
    i = 0;
    char *args = getenv("PATH");
    char **paths = ft_split(args, ':');
        while(paths[i])
        {
            char *str = ft_strjoin(paths[i], "/");
            char *end = ft_strjoin(str, pipe_blocs->acces_args->args[0]->str);
            if(access(end, X_OK) == 0)
            {
                execve(end, (char **)pipe_blocs->acces_args->args, get->envp);
                // free(str);
                // free(end);
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

int is_built_in(char *str)
{
    char *builtin[] = { "echo",
                    "pwd",
                    "cd",
                    "env",
                    "export",
                    "unset",
                    "exit",
                    NULL};
    int i;

    i = 0;
    while(builtin[i])
    {
        if(ft_strcmp(builtin[i], str) == 0)
            return 1;
        i++;
    }
    return 0;
}

void check_cmd_sys_call(t_general *pipe_blocs, t_env **env)
{
    pid_t pid;
    t_now *get;
    get = malloc(sizeof(t_now));
    get->envp = malloc(sizeof(t_now)*ft_lsttsize(*env));
    fill_env(env, get);
    pid = fork();

    if (pid == 0)
    {
        handle_output(pipe_blocs);
        handle_input(pipe_blocs);
        handle_append(pipe_blocs);
        handle_heredoc(pipe_blocs);
            if(is_built_in(pipe_blocs->acces_args->args[0]->str) == 1)
            {
                check_cmd_built_in(pipe_blocs, env);
                exit(pipe_blocs->dqm);
            }
            else
            {
                 execute_command(pipe_blocs, get);
                 exit(pipe_blocs->dqm);
            }

    }
    else
        waitpid(pid,NULL, 0);
}
