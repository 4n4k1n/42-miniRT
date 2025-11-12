/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_tile_result.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:31:43 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:44:42 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Receive a completed tile result from a worker.
 * Expects a MSG_TILE_COMPLETE header, reads the tile metadata (t_tile),
 * converts integer fields from network byte order to host order, allocates
 * a pixel buffer sized for the tile and reads the pixel data. The pixel
 * array returned is in host byte order (each uint32_t converted).
 * On allocation failure the function returns without writing to *pixels.
 * @param socket_fd connected socket for the worker
 * @param tile out parameter to receive tile metadata (width/height/x/y/tile_id)
 * @param pixels out pointer to newly allocated pixel buffer (caller must free)
 */
void	recive_tile_result(int socket_fd, t_tile *tile, uint32_t **pixels)
{
	t_msg_header	header;
	uint32_t		pixel_bytes;
	uint32_t		pixel_count;
	int				i;

	header = recive_header(socket_fd);
	if (header.msg_type != MSG_TILE_COMPLETE)
		printf("Warning: unexpected message type %d\n", header.msg_type);
	recv_all(socket_fd, tile, sizeof(t_tile));
	tile->height = ntohl(tile->height);
	tile->tile_id = ntohl(tile->tile_id);
	tile->width = ntohl(tile->width);
	tile->x = ntohl(tile->x);
	tile->y = ntohl(tile->y);
	pixel_count = tile->width * tile->height;
	pixel_bytes = pixel_count * sizeof(uint32_t);
	*pixels = malloc(pixel_bytes);
	if (!(*pixels))
		return ;
	recv_all(socket_fd, *pixels, pixel_bytes);
	i = -1;
	while (++i < (int)pixel_count)
		(*pixels)[i] = ntohl((*pixels)[i]);
}
