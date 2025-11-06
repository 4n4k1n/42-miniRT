/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claude <claude@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/10/24 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Ensures all bytes are sent through the socket.
 * Loops until the entire buffer is transmitted or an error occurs.
 * Returns 0 on success, -1 on error.
 */
int	send_all(int socket_fd, const void *buffer, size_t length)
{
	const char	*ptr;
	size_t		remaining;
	ssize_t		sent;

	ptr = (const char *)buffer;
	remaining = length;
	while (remaining > 0)
	{
		sent = send(socket_fd, ptr, remaining, 0);
		if (sent <= 0)
			return (-1);
		ptr += sent;
		remaining -= sent;
	}
	return (0);
}
