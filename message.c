/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:56:22 by bucolak           #+#    #+#             */
/*   Updated: 2025/05/10 14:21:47 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int message(int i)
{
    int k;
    if(i==0)
    {
        printf("%s","Çift tırnak kapanmadı.");
        k = 0;
    }
    else if (i==1)
    {
        printf("%s","Tek tırnak kapanmadı.");
        k = 0;
    }
    return k;
    
}