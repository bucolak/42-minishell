/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:30:51 by bucolak           #+#    #+#             */
/*   Updated: 2024/10/25 13:29:07 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	x;

	i = 0;
	str = (unsigned char *)s;
	x = (unsigned char)c;
	while (i < n)
	{
		if (x == str[i])
		{
			return (str + i);
		}
		i++;
	}
	return (NULL);
}
