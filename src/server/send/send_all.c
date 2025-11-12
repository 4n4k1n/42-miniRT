/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/11/12 17:16:38 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Send a compact update message (uint32 field) to a peer.
 * Constructs a t_update with the provided value converted to network byte
 * order, sends a MSG_UPDATE header followed by the payload.
 * @param socket_fd destination socket
 * @param update integer update value (host order)
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
