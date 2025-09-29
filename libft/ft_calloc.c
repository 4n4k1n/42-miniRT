/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:13:54 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 12:39:16 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// allocates memory and fills it with 0
void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = (void *)malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}

// #include <stdio.h>
// int main()
// {
// 	int *arr = (int*)ft_calloc(5, sizeof(int));
// 	if (arr == NULL) {
// 		printf("Allocation failed!\n");
// 		return 1;
// 	}
// 	printf("First element: %d\n", arr[0]); // Should print 0
// 	free(arr);
// 	return 0;
// }