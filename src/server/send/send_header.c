/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_header.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:46:10 by anakin            #+#    #+#             */
/*   Updated: 2025/11/07 10:42:19 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	send_header(int socket_fd, uint32_t msg_type, uint32_t payload)
{
	t_msg_header	header;

	header.msg_type = htonl(msg_type);
	header.payload_size = htonl(payload);
	return (send_all(socket_fd, &header, sizeof(header)));
}
