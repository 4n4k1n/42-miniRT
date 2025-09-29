/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:40:54 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 13:25:01 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// searches for the needle in the haystack and returns the first occurence
// it only searches up to len bytes
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	len_needle;
	size_t	i;
	size_t	j;

	len_needle = ft_strlen(needle);
	if (!haystack && !needle)
		return (NULL);
	if (len_needle == 0)
		return ((char *) haystack);
	i = 0;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (i + j < len && haystack[i + j] == needle[j]
			&& haystack[i + j] && needle[j])
			j++;
		if (j == len_needle)
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}
