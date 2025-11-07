/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_all.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/11/07 10:45:16 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Ensures all bytes are received from the socket.
 * Loops until the entire buffer is filled or an error occurs.
 * Returns 0 on success, -1 on error.
 */
int	recv_all(int socket_fd, void *buffer, size_t length)
{
	char	*ptr;
	size_t	remaining;
	ssize_t	received;

	ptr = (char *)buffer;
	remaining = length;
	while (remaining > 0)
	{
		received = recv(socket_fd, ptr, remaining, MSG_WAITALL);
		if (received < 0)
			return (-1);
		if (received == 0)
			return (-1);
		ptr += received;
		remaining -= received;
	}
	return (0);
}
