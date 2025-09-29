/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:50:20 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 12:38:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//allocates up to n bytes to 0
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

// #include <stdio.h>
// int main()
// {
// 	char str[10] = "HelloWorld";
// 	ft_bzero(str, 5);
// 	printf("Result: %s\n", str);
// 	return 0;
// }