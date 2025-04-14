/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:55:32 by bucolak           #+#    #+#             */
/*   Updated: 2024/10/25 13:10:01 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	unsigned const char	*s;

	d = (unsigned char *)dst;
	s = (unsigned const char *)src;
	if (!d && !s)
		return (NULL);
	if (d < s)
	{
		ft_memcpy(d, s, len);
	}
	else
	{
		d = d + (len);
		s = s + (len);
		while (len--)
		{
			*--d = *--s;
		}
	}
	return (dst);
}
