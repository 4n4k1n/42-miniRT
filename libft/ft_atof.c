/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:28:38 by nweber            #+#    #+#             */
/*   Updated: 2025/07/23 22:28:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static double	handle_decimal(const char *str, double result, int sign)
{
	double	fraction;

	fraction = 0.1;
	while (*str >= '0' && *str <= '9')
	{
		result += (*str - '0') * fraction * sign;
		fraction *= 0.1;
		str++;
	}
	return (result);
}

double	ft_atof(const char *str)
{
	double	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0') * sign;
		str++;
	}
	if (*str == '.')
		result = handle_decimal(str + 1, result, sign);
	return (result);
}
