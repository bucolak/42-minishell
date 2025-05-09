/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:51:00 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/09 20:10:19 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void renew_block(char ***args)
{
    int i = 0;
    int j = 0;
    char **new;
    while ((*args)[i])
        i++;
    new = malloc(sizeof(char *)*(i+1));
    i = 0;
    while(args[i])
    {
        if(ft_strcmp((*args)[i], "<") == 0
            || ft_strcmp((*args)[i], "<<") == 0
            || ft_strcmp((*args)[i], ">") == 0
            || ft_strcmp((*args)[i], ">>") == 0)
            i+=2;
        else
        {
            new[j] = ft_strdup((*args)[i]);
            j++;
        }
        i++;   
    }
    new[j] = NULL;
    
    *args = malloc (sizeof(t_arg *) * j+1);
    j = 0;
    i = 0;
    while(new[j])
    {
        (*args)[i] = ft_strdup(new[j]);
        j++;
        i++;
    }
    for (int k = 0; k < j; k++)
        free(new[k]);
    free(new);
    (*args)[i] = malloc(sizeof(t_arg));
    (*args)[i]= NULL;
}

void heredoc(t_general **list)
{
    t_general *tmp;
    int fd;
    struct stat path_stat;
    char *str;
    int i;
    tmp = *list;
    while (tmp)
    {
        i = 0;
        while(tmp->acces_args->args[i])
        {
            str = tmp->acces_args->args[i]->str;
            if(ft_strcmp(str, ">") == 0)
            {
                if(tmp->acces_args->args[i+1])
                {
                    i++;
                    if(stat(tmp->acces_args->args[i]->str, &path_stat) != 0)
                        fd = open(tmp->acces_args->args[i]->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    else
                        fd = open(tmp->acces_args->args[i]->str, O_WRONLY | O_TRUNC, 0644);
                    if (fd < 0)
                    {
                        perror("open");
                        return;
                    }
                    dup2(fd, 1);
                    close(fd);
                   // renew_block(tmp);
                }
            }
            i++;
        }
        tmp = tmp->next;
    }
}
