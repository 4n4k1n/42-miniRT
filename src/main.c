/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:00:01 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/01 23:08:37 by anakin           ###   ########.fr       */
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
        s2->data.sphere.mat = material_metal((t_rgb){204.0, 204.0, 204.0}, 0.0);
        obj_push(data->objects, s2);
    }

    // Center sphere: lambertian (0.1,0.2,0.5)
    s1 = obj_new(SPHERE);
    if (s1)
    {
        s1->data.sphere.cords = vec3_init_inline(0.0, 0.0, -1.2);
        s1->data.sphere.diameter = 1.0;
        s1->data.sphere.rgb = (t_rgb){255, 255, 255};
        s1->data.sphere.mat = material_metal((t_rgb){204.0, 204.0, 204.0}, 0.0);
        obj_push(data->objects, s1);
    }

    // Left sphere: metal (0.8,0.8,0.8), fuzz 0.3
    s3 = obj_new(SPHERE);
    if (s3)
    {
        s3->data.sphere.cords = vec3_init_inline(-1.0, 0.0, -1.0);
        s3->data.sphere.diameter = 1.0;
        s3->data.sphere.rgb = (t_rgb){255, 255, 255};
        s3->data.sphere.mat = material_metal((t_rgb){204.0, 153.0, 51.0}, 0.0);
        obj_push(data->objects, s3);
    }

    // Right sphere: metal (0.8,0.6,0.2), fuzz 1.0
    s4 = obj_new(SPHERE);
    if (s4)
    {
        s4->data.sphere.cords = vec3_init_inline(1.0, 0.0, -1.0);
        s4->data.sphere.diameter = 1.0;
        s4->data.sphere.rgb = (t_rgb){255, 255, 255};
        s4->data.sphere.mat = material_metal((t_rgb){204.0, 153.0, 51.0}, 0.0);
        obj_push(data->objects, s4);
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

void	update_camera(t_data *data)
{
	t_init_tmp tmp;

	tmp.t1 = vec3_init_inline(0.0, 0.0, data->camera.foc);
	tmp.t2 = vec3_sub_inline(&data->camera.cords, &tmp.t1);
	tmp.t3 = vec3_divide_inline(&data->camera.viewport_u, 2.0);
	tmp.t4 = vec3_sub_inline(&tmp.t2, &tmp.t3);
	tmp.t5 = vec3_divide_inline(&data->camera.viewport_v, 2.0);
	data->camera.viewport_upper_left = vec3_sub_inline(&tmp.t4, &tmp.t5);
	tmp.t6 = vec3_add_inline(&data->camera.pixel_delta_u, &data->camera.pixel_delta_v);
	tmp.t7 = vec3_multiply_inline(&tmp.t6, 0.5);
	data->camera.pixel00_loc = vec3_add_inline(&data->camera.viewport_upper_left, &tmp.t7);
}

/**
 * Key hook function to handle arrow key presses
 * Captures arrow key events and prints them to stdout
 */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if ((keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT) && (keydata.key == MLX_KEY_W
		|| keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_A
		|| keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_SPACE || keydata.key == MLX_KEY_LEFT_SHIFT || keydata.key == MLX_KEY_ESCAPE))
	{
		if (keydata.key == MLX_KEY_ESCAPE)
			mlx_close_window(data->mlx);
		else if (keydata.key == MLX_KEY_W)
			data->camera.cords.z -= 0.2;
		else if (keydata.key == MLX_KEY_S)
			data->camera.cords.z += 0.2;
		else if (keydata.key == MLX_KEY_A)
			data->camera.cords.x -= 0.2;
		else if (keydata.key == MLX_KEY_D)
			data->camera.cords.x += 0.2;
		else if (keydata.key == MLX_KEY_SPACE)
			data->camera.cords.y += 0.2;
		else if (keydata.key == MLX_KEY_LEFT_SHIFT)
			data->camera.cords.y -= 0.2;
		update_camera(data);
		render(data);
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
	int	render_time;

	(void)ac;
	(void)av;
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
