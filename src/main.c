/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/30 17:07:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	build_demo_world(t_data *data)
{
	t_obj	*s1;
	t_obj	*s2;

	data->objects = (t_obj_list *)malloc(sizeof(t_obj_list));
	if (!data->objects)
		return;
	data->objects->head = NULL;
	data->objects->tail = NULL;
	data->objects->size = 0;

	// Sphere at (0,0,-1), radius 0.5 -> diameter 1.0
	s1 = obj_new(SPHERE);
	if (s1)
	{
		s1->data.sphere.cords = vec3_init_inline(0.0, 0.0, -1.0);
		s1->data.sphere.diameter = 1.0;
		s1->data.sphere.rgb = (t_rgb){255, 255, 255};
		obj_push(data->objects, s1);
	}

	// Ground sphere at (0,-100.5,-1), radius 100 -> diameter 200
	s2 = obj_new(SPHERE);
	if (s2)
	{
		s2->data.sphere.cords = vec3_init_inline(0.0, -100.5, -1.0);
		s2->data.sphere.diameter = 200.0;
		s2->data.sphere.rgb = (t_rgb){255, 255, 255};
		obj_push(data->objects, s2);
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	(void)ac;
	(void)av;
	
	init_camera(&data);
	build_demo_world(&data);

	mlx_set_setting(MLX_MAXIMIZED, false);
	data.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!data.mlx)
		return (1);
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		return (1);

	render(&data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	return (0);
}
