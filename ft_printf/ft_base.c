/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:58:31 by bucolak           #+#    #+#             */
/*   Updated: 2024/11/03 15:18:00 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*control(int basenum, int s)
{
	char	*base;

	if (basenum == 10 && s == 2)
		base = "0123456789";
	if (basenum == 16)
	{
		if (s == 0)
			base = "0123456789abcdef";
		if (s == 1)
			base = "0123456789ABCDEF";
	}
	return (base);
}

int	ft_base(int basenum, long num, int s)
{
	int		len;
	char	*base;

	len = 0;
	base = control(basenum, s);
	if (num < 0 && basenum == 10)
	{
		len += ft_putchr_pt('-');
		num = -num;
	}
	if (num >= basenum)
	{
		len += ft_base(basenum, num / basenum, s);
	}
	len += ft_putchr_pt(base[(num % basenum)]);
	return (len);
}
