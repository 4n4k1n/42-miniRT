/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:56:37 by nweber            #+#    #+#             */
/*   Updated: 2025/07/07 13:10:50 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// puts a char at file descriptor
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
