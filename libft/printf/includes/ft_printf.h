/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:21:59 by nweber            #+#    #+#             */
/*   Updated: 2025/07/26 09:20:39 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>

# if defined(__linux__)
#  define NULL_PTR "(nil)"
#  define NULL_PTR_LEN 5
# elif defined(__APPLE__)
#  define NULL_PTR "0x0"
#  define NULL_PTR_LEN 3
# endif

// Mandatory functions
int	ft_putchar(char c);
int	ft_putstr(char *str);
int	convert_hex(unsigned int n, char *charset);
int	putnbr_unsigned(unsigned int n);
int	putnbr(int n);
int	convert_pointer(void *p);
int	ft_printf(const char *frmt, ...);

#endif