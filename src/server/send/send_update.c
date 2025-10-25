/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:36:16 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/24 21:22:30 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	send_update(int socket_fd, uint32_t update)
{
	t_update	net_update;
	
	send_header(socket_fd, MSG_UPDATE, sizeof(t_update));
	net_update.updated_varible = htonl(update);
	send_all(socket_fd, &net_update, sizeof(t_update));
}
