/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:51:58 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:43:51 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Receive an update message from the socket.
 * Reads the message header and expects MSG_UPDATE. Then reads the update
 * payload into a t_update structure and converts fields from network to
 * host byte order where necessary before returning it.
 * @param socket_fd connected socket file descriptor
 * @return fully populated t_update (caller receives by value)
 */
t_update	recive_update(int socket_fd)
{
	t_msg_header	header;
	t_update		update;

	header = recive_header(socket_fd);
	if (header.msg_type != MSG_UPDATE)
		printf("Warning: unexpected message type %d\n", header.msg_type);
	recv_all(socket_fd, &update, sizeof(t_update));
	update.updated_varible = ntohl(update.updated_varible);
	return (update);
}
