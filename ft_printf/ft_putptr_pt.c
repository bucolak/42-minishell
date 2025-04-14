/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_pt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:59:01 by bucolak           #+#    #+#             */
/*   Updated: 2024/11/03 15:49:02 by bucolak          ###   ########.fr       */
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

static int	ft_baseu(unsigned long basenum, unsigned long num, int s)
{
	int		len;
	char	*base;

	len = 0;
	base = control(basenum, s);
	if (num >= basenum)
	{
		len += ft_base(basenum, num / basenum, s);
	}
	len += ft_putchr_pt(base[(num % basenum)]);
	return (len);
}

int	ft_putptr_pt(void *ptr)
{
	unsigned long	ptr_adr;
	int				len;

	len = 0;
	ptr_adr = (unsigned long)ptr;
	len += ft_baseu(16, ptr_adr, 0);
	return (len + 2);
}
