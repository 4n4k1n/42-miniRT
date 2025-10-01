/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/01 02:28:47 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Creates a demo scene with two spheres for testing
 * Sets up object list and adds one small sphere and one large ground sphere
 * Used to test ray tracing functionality
 */
static void	build_demo_world(t_data *data)
{
    t_obj	*s1;
    t_obj	*s2;
    // NEW demo spheres
    t_obj	*s3;
    t_obj	*s4;

    data->objects = (t_obj_list *)malloc(sizeof(t_obj_list));
    if (!data->objects)
        return;
    data->objects->head = NULL;
    data->objects->tail = NULL;
    data->objects->size = 0;

    // Ground sphere: lambertian (0.8,0.8,0.0)
    s2 = obj_new(SPHERE);
    if (s2)
    {
        s2->data.sphere.cords = vec3_init_inline(0.0, -100.5, -1.0);
        s2->data.sphere.diameter = 200.0;
        s2->data.sphere.rgb = (t_rgb){255, 255, 255};
        s2->data.sphere.mat = material_lambertian((t_rgb){204.0, 204.0, 0.0}); // 0.8*255
        obj_push(data->objects, s2);
    }

    // Center sphere: lambertian (0.1,0.2,0.5)
    s1 = obj_new(SPHERE);
    if (s1)
    {
        s1->data.sphere.cords = vec3_init_inline(0.0, 0.0, -1.2);
        s1->data.sphere.diameter = 1.0;
        s1->data.sphere.rgb = (t_rgb){255, 255, 255};
        s1->data.sphere.mat = material_lambertian((t_rgb){25.5, 51.0, 127.5}); // 0.1,0.2,0.5
        obj_push(data->objects, s1);
    }

    // Left sphere: metal (0.8,0.8,0.8), fuzz 0.3
    s3 = obj_new(SPHERE);
    if (s3)
    {
        s3->data.sphere.cords = vec3_init_inline(-1.0, 0.0, -1.0);
        s3->data.sphere.diameter = 1.0;
        s3->data.sphere.rgb = (t_rgb){255, 255, 255};
        s3->data.sphere.mat = material_metal((t_rgb){204.0, 204.0, 204.0}, 0.3);
        obj_push(data->objects, s3);
    }

    // Right sphere: metal (0.8,0.6,0.2), fuzz 1.0
    s4 = obj_new(SPHERE);
    if (s4)
    {
        s4->data.sphere.cords = vec3_init_inline(1.0, 0.0, -1.0);
        s4->data.sphere.diameter = 1.0;
        s4->data.sphere.rgb = (t_rgb){255, 255, 255};
        s4->data.sphere.mat = material_metal((t_rgb){204.0, 153.0, 51.0}, 1.0);
        obj_push(data->objects, s4);
    }
}

/**
 * Main entry point of the miniRT ray tracer
 * Initializes camera, creates demo world, sets up MLX window
 * Renders the scene and starts the main loop
 */
int	main(int ac, char **av)
{
	t_data	data;

	(void)ac;
	(void)av;
	data.camera.samples_per_pixel = AA_MAX_SAMPLES; // AA
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
