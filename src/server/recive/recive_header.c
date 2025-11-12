/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_header.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:51:43 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:45:43 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Receive a message header from the socket and convert to host order.
 * Uses recv_all to ensure the header is fully read. Converts msg_type and
 * payload_size from network byte order to host byte order before returning.
 * @param socket_fd connected socket descriptor
 * @return t_msg_header with fields in host byte order
 */
t_msg_header	recive_header(int socket_fd)
{
	t_msg_header	header;

	recv_all(socket_fd, &header, sizeof(t_msg_header));
	header.msg_type = ntohl(header.msg_type);
	header.payload_size = ntohl(header.payload_size);
	return (header);
}
