/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:02:36 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 12:29:32 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Converts a str into a int
int	ft_atoi(const char *str)
{
	int	i;
	int	mult;
	int	result;

	i = 0;
	mult = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			mult = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * mult);
}

// #include <stdio.h>
// int	main(void)
// {
// 	printf("Test 1: ft_atoi(\"  123\") = %d\n", ft_atoi("  123"));
// 	printf("Test 2: ft_atoi(\"-123\") = %d\n", ft_atoi("-123"));
// 	printf("Test 3: ft_atoi(\"+123\") = %d\n", ft_atoi("+123"));
// }