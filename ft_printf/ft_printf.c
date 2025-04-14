/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:10:36 by bucolak           #+#    #+#             */
/*   Updated: 2024/11/03 13:00:28 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_form(const char format, va_list va)
{
	if (format == 'c')
		return (ft_putchr_pt(va_arg(va, int)));
	else if (format == 's')
		return (ft_putstr_pt(va_arg(va, char *)));
	else if (format == 'p')
	{
		write(1, "0x", 2);
		return (ft_putptr_pt(va_arg(va, void *)));
	}
	else if (format == 'd' || format == 'i')
		return (ft_base(10, va_arg(va, int), 2));
	else if (format == 'u')
		return (ft_base(10, (unsigned int)va_arg(va, int), 2));
	else if (format == 'x')
		return (ft_base(16, (unsigned int)va_arg(va, int), 0));
	else if (format == 'X')
		return (ft_base(16, (unsigned int)va_arg(va, int), 1));
	else if (format == '%')
		return (ft_putchr_pt('%'));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		j;
	va_list	va;

	i = 0;
	j = 0;
	va_start(va, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			i++;
			j += ft_form(str[i], va);
		}
		else
		{
			j += write(1, &str[i], 1);
		}
		i++;
	}
	va_end(va);
	return (j);
}
