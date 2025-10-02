/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/02 11:43:11 by anakin           ###   ########.fr       */
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
    t_obj	*o;
    int		i;
    int		ring_count;
    double	angle;
    double	radius;

    data->objects = (t_obj_list *)malloc(sizeof(t_obj_list));
    if (!data->objects)
        return;
    data->objects->head = NULL;
    data->objects->tail = NULL;
    data->objects->size = 0;

    /* Epic metallic ground (big low-fuzz mirror) */
    o = obj_new(SPHERE);
    if (o)
    {
        o->data.sphere.cords = vec3_init_inline(0.0, -1001.0, -1.0);
        o->data.sphere.diameter = 2002.0;
        o->data.sphere.rgb = (t_rgb){255, 255, 255};
        o->data.sphere.mat = material_metal((t_rgb){220.0, 225.0, 230.0}, 0.01);
        obj_push(data->objects, o);
    }

    /* Central polished orb (near-perfect chrome) */
    o = obj_new(SPHERE);
    if (o)
    {
        o->data.sphere.cords = vec3_init_inline(0.0, 0.6, -1.5);
        o->data.sphere.diameter = 1.6;
        o->data.sphere.rgb = (t_rgb){255, 255, 255};
        o->data.sphere.mat = material_metal((t_rgb){235.0, 235.0, 240.0}, 0.0);
        obj_push(data->objects, o);
    }

    /* Inner golden core (slightly rough gold) */
    o = obj_new(SPHERE);
    if (o)
    {
        o->data.sphere.cords = vec3_init_inline(0.0, 0.6, -1.5);
        o->data.sphere.diameter = 0.7;
        o->data.sphere.rgb = (t_rgb){255, 215, 0};
        o->data.sphere.mat = material_metal((t_rgb){255.0, 200.0, 80.0}, 0.04);
        obj_push(data->objects, o);
    }

    /* Ring of metallic satellites around the orb */
    ring_count = 14;
    radius = 3.2;
    for (i = 0; i < ring_count; ++i)
    {
        angle = (2.0 * M_PI * i) / (double)ring_count;
        o = obj_new(SPHERE);
        if (!o)
            continue;
        o->data.sphere.cords = vec3_init_inline(cos(angle) * radius, 0.9 + 0.3 * sin(3*angle), sin(angle) * radius - 1.5);
        o->data.sphere.diameter = 0.45;
        o->data.sphere.rgb = (t_rgb){200, 200, 220};
        /* vary albedo and fuzz for visual variety */
        o->data.sphere.mat = material_metal((t_rgb){180.0 + (i % 3) * 20.0, 180.0 + (i % 5) * 12.0, 200.0}, 0.02 + (i % 4) * 0.02);
        obj_push(data->objects, o);
    }

    /* A few tall metallic spires (stacked spheres) */
    for (i = 0; i < 3; ++i)
    {
        int j;
        double cx = -5.5 + i * 5.5;
        for (j = 0; j < 4; ++j)
        {
            o = obj_new(SPHERE);
            if (!o)
                continue;
            o->data.sphere.cords = vec3_init_inline(cx, 0.25 + j * 0.6, -3.0);
            o->data.sphere.diameter = 0.5 - j * 0.06;
            o->data.sphere.rgb = (t_rgb){220, 210, 200};
            o->data.sphere.mat = material_metal((t_rgb){200.0 + j * 10.0, 190.0 + j * 8.0, 180.0}, 0.03 + j * 0.02);
            obj_push(data->objects, o);
        }
    }
}

int	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	cleanup_objects(t_obj_list *objects)
{
	t_obj	*current;
	t_obj	*next;

	if (!objects)
		return ;
	current = objects->head;
	while (current)
	{
		next = current->next;
		if (current->type == SPHERE && current->data.sphere.mat)
			free(current->data.sphere.mat);
		free(current);
		current = next;
	}
	free(objects);
}

/**
 * Main entry point of the miniRT ray tracer
 * Initializes camera, creates demo world, sets up MLX window
 * Renders the scene and starts the main loop
 */
int	main(int ac, char **av)
{
	t_data	data;
	int	render_time;

	(void)ac;
	(void)av;
    data.aa_state = ANTI_ALIASING;
	data.camera.samples_per_pixel = AA_MAX_SAMPLES;
	init_camera(&data);
	build_demo_world(&data);
	mlx_set_setting(MLX_MAXIMIZED, false);
	data.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!data.mlx)
		return (1);
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		return (1);
	mlx_key_hook(data.mlx, key_hook, &data);
	render_time = get_time_in_ms();
	render(&data);
	printf("\n%.2f sec\n", (float)(get_time_in_ms() - render_time) / 1000);
	mlx_loop(data.mlx);
	cleanup_objects(data.objects);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}
