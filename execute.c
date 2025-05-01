/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:47:09 by buket             #+#    #+#             */
/*   Updated: 2025/04/26 22:35:32 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void check_cmd_sys_call(t_general *pipe_blocs)
// {
//     char *args = getenv("PATH");
//     char **paths = ft_split(args, ':');
//     int i;
//     int j;
//     while(pipe_blocs)
//     {
//         i = 0;
//         while(pipe_blocs->acces_args->args[i])
//         {
//             j = 0;
//             while(paths[j])
//             {
//                 while(paths[j])
//                     j++;
//                 paths[j] = '/';
//                 j = 0;
//                 char *p = ft_strjoin(paths[j], pipe_blocs->acces_args->args[i]->str);
//                 if(access(p, X_OK))
//                 {
//                     execve(p, pipe_blocs->acces_args->args[i]->str, args);
//                     break;
//                 }
//                 j++;
//             }
//             i++;
//         }
//         pipe_blocs = pipe_blocs->next;
//     }
    
// }