/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putnbr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:19:09 by nweber            #+#    #+#             */
/*   Updated: 2025/07/08 11:54:17 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int	putnbr_rec(long nb)
{
	int		len;
	int		ret;
	char	c;

	len = 0;
	if (nb >= 10)
	{
		ret = putnbr_rec(nb / 10);
		if (ret == -1)
			return (-1);
		len += ret;
	}
	c = '0' + (nb % 10);
	ret = write(1, &c, 1);
	if (ret == -1)
		return (-1);
	return (len + ret);
}

int	putnbr(int n)
{
	long	nb;
	int		len;
	int		ret;

	nb = n;
	len = 0;
	if (nb < 0)
	{
		ret = write(1, "-", 1);
		if (ret == -1)
			return (-1);
		len += ret;
		nb = -nb;
	}
	ret = putnbr_rec(nb);
	if (ret == -1)
		return (-1);
	return (len + ret);
}

int	putnbr_unsigned(unsigned int n)
{
	int		ret;
	int		len;
	char	c;

	len = 0;
	if (n >= 10)
	{
		ret = putnbr_unsigned(n / 10);
		if (ret == -1)
			return (-1);
		len += ret;
	}
	c = (n % 10) + '0';
	ret = write(1, &c, 1);
	if (ret == -1)
		return (-1);
	len += ret;
	return (len);
}

int	convert_hex(unsigned int n, char *charset)
{
	int	ret;
	int	len;

	len = 0;
	if (n >= 16)
	{
		ret = convert_hex(n / 16, charset);
		if (ret == -1)
			return (-1);
		len += ret;
	}
	ret = write(1, &charset[n % 16], 1);
	if (ret == -1)
		return (-1);
	len += ret;
	return (len);
}
