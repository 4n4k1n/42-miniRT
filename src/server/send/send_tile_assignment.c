/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_tile_assignment.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:05:52 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:15:21 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Create, bind and listen on a TCP socket on the specified port.
 * Sets SO_REUSEADDR on the socket and starts listening with a backlog.
 * Returns the listening socket file descriptor on success, or a negative
 * value on failure.
 * @param port TCP port to bind to (host order)
 * @return listening socket fd or negative on error
 */
void	send_tile_assignment(int socket_fd, t_tile *tile)
{
	t_tile	net_tile;

	send_header(socket_fd, MSG_RENDER_TILE, sizeof(t_tile));
	net_tile.tile_id = htonl(tile->tile_id);
	net_tile.x = htonl(tile->x);
	net_tile.y = htonl(tile->y);
	net_tile.width = htonl(tile->width);
	net_tile.height = htonl(tile->height);
	send_all(socket_fd, &net_tile, sizeof(t_tile));
}
