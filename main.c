/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:22:33 by bucolak           #+#    #+#             */
/*   Updated: 2025/04/14 16:34:34 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_cmd()
{
    
}

int main()
{
    char *line;
    if(line = readline("Our_shell >")==-1)
        ft_printf("Error\n");
    add_history(line);
    
    
}