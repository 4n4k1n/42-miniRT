/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:51:58 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/13 18:55:32 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_update	recive_update(int socket_fd)
{
	t_msg_header	header;
	t_update		update;

	header = recive_header(socket_fd);
	recv(socket_fd, &update, sizeof(t_update), 0);
	update.updated_varible = ntohl(update.updated_varible);
	return (update);
}
