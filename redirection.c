/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:51:00 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/11 18:31:24 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void print_args(t_arg **args)
// {
//     int i = 0;

//     printf("AYIKLANMIŞ ARGÜMANLAR:\n");
//     while (args && args[i])
//     {
//         printf("Arg[%d]: \"%s\", Flag: %d\n", i, args[i]->str, args[i]->flag);
//         i++;
//     }
// }

void renew_block2(t_general *list)
{
    int i = 0, j = 0;
    t_arg **new;
    t_general *tmp = list;

    while (tmp->acces_args->args[i])
        i++;
    new = malloc(sizeof(t_arg *) * (i + 1));

    i = 0;
    while (tmp->acces_args->args[i])
    {
        if (ft_strcmp(tmp->acces_args->args[i]->str, "<") == 0 ||
            ft_strcmp(tmp->acces_args->args[i]->str, "<<") == 0 ||
            ft_strcmp(tmp->acces_args->args[i]->str, ">") == 0 ||
            ft_strcmp(tmp->acces_args->args[i]->str, ">>") == 0)
        {
            i += 2;
        }
        else
        {
            new[j] = malloc(sizeof(t_arg));
            new[j]->str = ft_strdup(tmp->acces_args->args[i]->str);
            new[j]->flag = tmp->acces_args->args[i]->flag;
            j++;
            i++;
        }
    }
    new[j] = NULL;
    tmp->acces_args->args = new;
    //print_args(tmp->acces_args->args);
}


void renew_block(char ***args)
{
    int i = 0;
    int j = 0;
    char **new;
    char **old;
    old = *args;
    while (old[i])
        i++;
    new = malloc(sizeof(char *)*(i+1));
    i = 0;
    while(old[i])
    {
        if(ft_strcmp(old[i], "<") == 0
            || ft_strcmp(old[i], "<<") == 0
            || ft_strcmp(old[i], ">") == 0
            || ft_strcmp(old[i], ">>") == 0)
            i+=2;
        else
        {
            new[j] = ft_strdup(old[i]);
            j++;
        }
        i++;   
    }
    new[j] = NULL;
    *args = new;
}

// void heredoc(t_general **list)
// {
//     t_general *tmp;
//     int fd;
//     struct stat path_stat;
//     char *str;
//     int i;
//     tmp = *list;
//     while (tmp)
//     {
//         i = 0;
//         while(tmp->acces_args->args[i])
//         {
//             str = tmp->acces_args->args[i]->str;
//             if(ft_strcmp(str, ">") == 0)
//             {
//                 if(tmp->acces_args->args[i+1])
//                 {
//                     i++;
//                     if(stat(tmp->acces_args->args[i]->str, &path_stat) != 0)
//                         fd = open(tmp->acces_args->args[i]->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
//                     else
//                         fd = open(tmp->acces_args->args[i]->str, O_WRONLY | O_TRUNC, 0644);
//                     if (fd < 0)
//                     {
//                         perror("open");
//                         return;
//                     }
//                     dup2(fd, 1);
//                     close(fd);
//                    // renew_block(tmp);
//                 }
//             }
//             i++;
//         }
//         tmp = tmp->next;
//     }
// }
