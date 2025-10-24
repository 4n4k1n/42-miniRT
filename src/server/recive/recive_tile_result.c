/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_tile_result.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:31:43 by anakin            #+#    #+#             */
/*   Updated: 2025/10/11 22:43:21 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void    recive_tile_result(int socket_fd, t_tile *tile, uint32_t **pixels)
{
    t_msg_header    header;
    uint32_t        pixel_bytes;

    header = recive_header(socket_fd);
    if (header.msg_type != MSG_TILE_COMPLETE)
        printf("Warning: unexpected message type %d\n", header.msg_type);
    recv_all(socket_fd, tile, sizeof(t_tile));
    tile->height = ntohl(tile->height);
    tile->tile_id = ntohl(tile->tile_id);
    tile->width = ntohl(tile->width);
    tile->x = ntohl(tile->x);
    tile->y = ntohl(tile->y);
    pixel_bytes = tile->width * tile->height * sizeof(uint32_t);
    *pixels = malloc(pixel_bytes);
    if (!(*pixels))
        return ;
    recv_all(socket_fd, *pixels, pixel_bytes);
}
