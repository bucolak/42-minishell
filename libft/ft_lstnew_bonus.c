/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 09:25:23 by bucolak           #+#    #+#             */
/*   Updated: 2024/10/25 13:10:53 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*str;

	str = (t_list *)malloc(sizeof(t_list));
	if (!str)
		return (NULL);
	str->content = content;
	str->next = NULL;
	return (str);
}
