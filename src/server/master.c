/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 11:41:09 by anakin            #+#    #+#             */
/*   Updated: 2025/10/12 12:05:43 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int setup_master(t_master *master, t_data *data, char *scene_file, uint32_t port)
{
    ft_memset(master, 0, sizeof(t_master));
    master->scene_file = scene_file;
    parse_scene(scene_file, data);
    init_queue(&master->queue, WIDTH, HEIGHT, TILE_SIZE);
    master->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    master->img = mlx_new_image(master->mlx, WIDTH, HEIGHT);
    mlx_image_to_window(master->mlx, master->img, 0, 0);
    pthread_mutex_init(&master->img_lock, NULL);
    master->socket_fd = setup_listen_socket(port);
    pthread_create(&master->accept_thread, NULL, accept_worker_thread, &master);
}

int run_master(char *scene_file, uint32_t port)
{
    t_master    master;
    t_data      data;

    setup_master(&master, &data, scene_file, port);
}
