/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_tile_result.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:24:27 by anakin            #+#    #+#             */
/*   Updated: 2025/10/11 21:57:36 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void    send_tile_result(int socket_fd, t_tile *tile_result, uint32_t *pixels)
{
    uint32_t    pixel_bytes;
    t_tile      net_tile;
    
    pixel_bytes = tile_result->height * tile_result->width * sizeof(uint32_t);
    send_header(socket_fd, MSG_TILE_COMPLETE, sizeof(t_tile) + pixel_bytes);
    net_tile.height = htonl(tile_result->height);
    net_tile.tile_id = htonl(tile_result->tile_id);
    net_tile.width = htonl(tile_result->width);
    net_tile.x = htonl(tile_result->x);
    net_tile.y = htonl(tile_result->y);
    send(socket_fd, &net_tile, sizeof(t_tile), 0);
    send(socket_fd, pixels, pixel_bytes, 0);
}
