/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bucolak <bucolak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:36:35 by bucolak           #+#    #+#             */
/*   Updated: 2024/11/03 15:41:44 by bucolak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *str, ...);
int	ft_putchr_pt(char c);
int	ft_base(int basenum, long num, int s);
int	ft_putstr_pt(char *str);
int	ft_putptr_pt(void *ptr);

#endif