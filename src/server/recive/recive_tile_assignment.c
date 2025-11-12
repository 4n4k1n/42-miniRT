/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_tile_assignment.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:20:27 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:44:52 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Receive a tile assignment from the master.
 * Reads a t_tile structure from the socket and converts integer fields from
 * network byte order to host byte order before returning the tile.
 * @param socket_fd connected socket descriptor
 * @return t_tile with fields converted to host order
 */
t_tile	recive_tile_assignment(int socket_fd)
{
	t_tile	tile;

	recv_all(socket_fd, &tile, sizeof(t_tile));
	tile.height = ntohl(tile.height);
	tile.tile_id = ntohl(tile.tile_id);
	tile.width = ntohl(tile.width);
	tile.x = ntohl(tile.x);
	tile.y = ntohl(tile.y);
	return (tile);
}
