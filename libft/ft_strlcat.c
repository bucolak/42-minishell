/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:19:25 by bucolak           #+#    #+#             */
/*   Updated: 2024/10/25 13:13:33 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dstl;

	dstl = ft_strlen(dst);
	i = 0;
	j = dstl;
	if (dstsize <= dstl)
	{
		return (dstsize + ft_strlen(src));
	}
	while (src[i] != '\0' && i < (dstsize - dstl - 1))
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (dstl + ft_strlen(src));
}
