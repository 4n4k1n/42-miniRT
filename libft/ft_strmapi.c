/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:46:46 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 13:23:14 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// applies the function f to the string and creates a new one
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*result;

	if (!s || !f)
		return (NULL);
	result = (char *)malloc(ft_strlen(s) + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (s[i])
	{
		result[i] = f(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
