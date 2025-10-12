/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_tile_assignment.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:05:52 by anakin            #+#    #+#             */
/*   Updated: 2025/10/11 14:14:27 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void    send_tile_assignment(int socket_fd, t_tile *tile)
{
    t_tile  net_tile;

    send_header(socket_fd, MSG_RENDER_TILE, sizeof(t_tile));
    net_tile.height = htonl(tile->height);
    net_tile.tile_id = htonl(tile->tile_id);
    net_tile.width = htonl(tile->width);
    net_tile.x = htonl(tile->x);
    net_tile.y = htonl(tile->y);
    send(socket_fd, &net_tile, sizeof(t_tile), 0);
}
