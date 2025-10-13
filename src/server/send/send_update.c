/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:36:16 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/13 18:45:45 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	send_update(int socket_fd, t_update *update)
{
	t_update	net_update;
	
	send_header(socket_fd, MSG_UPDATE, sizeof(t_update));
	net_update.updated_varible = htonl(net_update.updated_varible);
	send(socket_fd, &net_update, sizeof(t_update), 0);
}
