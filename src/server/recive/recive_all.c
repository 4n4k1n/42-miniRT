/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_all.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/11/12 16:45:59 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Ensure all bytes are received into the provided buffer.
 * Repeatedly calls recv with MSG_WAITALL until the requested length is read
 * or an error/EOF occurs. Returns 0 on success and -1 on error.
 * @param socket_fd socket to read from
 * @param buffer destination buffer to fill
 * @param length number of bytes to receive
 * @return 0 on success, -1 on error or EOF
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
