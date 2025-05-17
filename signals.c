/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:16:35 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/17 17:05:03 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	cont;

void	handle_SIGINT(int signo)
{
    (void)signo;
    write(1, "\n", 1); 
    rl_on_new_line();
    //rl_replace_line("", 0);
	rl_redisplay();
}
