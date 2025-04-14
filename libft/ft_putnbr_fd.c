/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:54:56 by bucolak           #+#    #+#             */
/*   Updated: 2024/10/25 13:09:31 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
	{
		if (n == -2147483648)
			write(fd, "-2147483648", 11);
		else
		{
			ft_putchar_fd('-', fd);
			n = -n;
			ft_putnbr_fd(n, fd);
		}
	}
	else if (n == 0)
		ft_putchar_fd('0', fd);
	else
	{
		if (n <= 9)
		{
			ft_putchar_fd(n + '0', fd);
		}
		else
		{
			ft_putnbr_fd(n / 10, fd);
			ft_putchar_fd((n % 10) + '0', fd);
		}
	}
}
