/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_tile_result.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:24:27 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:15:32 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Send a completed tile result back to the master/receiver.
 * Allocates a network-order copy of the pixel buffer, converts each pixel to
 * network byte order, sends a MSG_TILE_COMPLETE header with total payload
 * size (t_tile + pixel bytes), then transmits the tile metadata and pixels.
 * Frees the temporary buffer on completion.
 * @param socket_fd connected socket to send to
 * @param tile_result tile metadata in host byte order
 * @param pixels pointer to host-order pixel buffer (uint32_t per pixel)
 */
void	send_tile_result(int socket_fd, t_tile *tile_result, uint32_t *pixels)
{
	uint32_t	pixel_bytes;
	uint32_t	pixel_count;
	uint32_t	*net_pixels;
	int			i;
	t_tile		net_tile;

	pixel_count = tile_result->height * tile_result->width;
	pixel_bytes = pixel_count * sizeof(uint32_t);
	net_pixels = malloc(pixel_bytes);
	if (!net_pixels)
		return ;
	i = -1;
	while (++i < (int)pixel_count)
		net_pixels[i] = htonl(pixels[i]);
	send_header(socket_fd, MSG_TILE_COMPLETE, sizeof(t_tile) + pixel_bytes);
	net_tile.tile_id = htonl(tile_result->tile_id);
	net_tile.x = htonl(tile_result->x);
	net_tile.y = htonl(tile_result->y);
	net_tile.width = htonl(tile_result->width);
	net_tile.height = htonl(tile_result->height);
	send_all(socket_fd, &net_tile, sizeof(t_tile));
	send_all(socket_fd, net_pixels, pixel_bytes);
	free(net_pixels);
}
