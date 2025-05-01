/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:14:07 by bucolak           #+#    #+#             */
/*   Updated: 2025/04/30 17:18:37 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_dquote(char *str)
{
    int i = 0;
    int c = 0;
    while(str[i])
    {
        if(str[i] == '"')
            c++;
        i++;
    }
    return c;
}

int count_squote(char *str)
{
    int i = 0;
    int c = 0;
    while(str[i])
    {
        if(str[i] == '\'')
            c++;
        i++;
    }
    return c;
}