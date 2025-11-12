/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_header.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:46:10 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:17:14 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Send a simple message header (type + payload size) in network byte order.
 * This is a thin wrapper that fills a t_msg_header, converts fields to
 * network byte order and transmits it using send_all.
 * @param socket_fd destination socket
 * @param msg_type message type (host order)
 * @param payload payload size in bytes (host order)
 */
void	send_header(int socket_fd, uint32_t msg_type, uint32_t payload)
{
	t_msg_header	header;

	header.msg_type = htonl(msg_type);
	header.payload_size = htonl(payload);
	send_all(socket_fd, &header, sizeof(header));
}
