/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_tile_assignment.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:20:27 by anakin            #+#    #+#             */
/*   Updated: 2025/10/11 22:27:57 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_tile  recive_tile_assignment(int socket_fd)
{
    t_msg_header    header;
    t_tile          tile;

    header = recive_header(socket_fd);
    recv(socket_fd, &tile, sizeof(t_tile), MSG_WAITALL);
    tile.height = ntohl(tile.height);
    tile.tile_id = ntohl(tile.tile_id);
    tile.width = ntohl(tile.width);
    tile.x = ntohl(tile.x);
    tile.y = ntohl(tile.y);
    return (tile);
}
