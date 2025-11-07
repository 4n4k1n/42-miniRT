/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_header.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:51:43 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 09:47:22 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_msg_header	recive_header(int socket_fd)
{
	t_msg_header	header;

	recv_all(socket_fd, &header, sizeof(t_msg_header));
	header.msg_type = ntohl(header.msg_type);
	header.payload_size = ntohl(header.payload_size);
	return (header);
}
