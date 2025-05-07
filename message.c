/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buket <buket@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:56:22 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/07 14:53:17 by buket            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int message(int i)
{
    if(i==0)
    {
        printf("%s","Çift tırnak kapanmadı.");
        return 0;
    }
    else if (i==1)
    {
        printf("%s","Tek tırnak kapanmadı.");
        return 0;
    }
    
}