/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:16:35 by bucolak           #+#    #+#             */
/*   Updated: 2025/07/02 21:35:06 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int signo)
{
    if(signo == SIGINT)
    {
        write(1, "\n", 1); 
        rl_on_new_line();
       // rl_replace_line("", 0);
	    rl_redisplay();
    }
}