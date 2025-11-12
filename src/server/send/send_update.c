/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:36:16 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 17:15:44 by nweber           ###   ########.fr       */
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
void	send_update(int socket_fd, uint32_t update)
{
	t_update	net_update;

	send_header(socket_fd, MSG_UPDATE, sizeof(t_update));
	net_update.updated_varible = htonl(update);
	send_all(socket_fd, &net_update, sizeof(t_update));
}
