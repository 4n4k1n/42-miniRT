/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:32:50 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 13:18:13 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// allocates memory and returns a string combining s1 and s2
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;

	str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	if (s1)
		ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	else
		str[0] = '\0';
	if (s2)
	{
		ft_strlcpy(str + ft_strlen(s1), s2, ft_strlen(s2) + 1);
	}
	else
		str[ft_strlen(s1)] = '\0';
	return (str);
}
