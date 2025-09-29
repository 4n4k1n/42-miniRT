/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:03:38 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 13:11:36 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// puts out a stringat file descriptor followed by a newline
void	ft_putendl_fd(char *s, int fd)
{
	if (s)
	{
		ft_putstr_fd(s, fd);
		ft_putchar_fd('\n', fd);
	}
}
