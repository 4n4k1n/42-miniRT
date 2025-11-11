/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:31:15 by anakin            #+#    #+#             */
/*   Updated: 2025/11/11 22:41:34 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	render_with_mt(t_data *data)
{
	int	checked;

	checked = 0;
	__atomic_store_n(&data->threads_done, 0, __ATOMIC_SEQ_CST);
	change_thread_state(data);
	while (1)
	{
		checked = __atomic_load_n(&data->threads_done, __ATOMIC_SEQ_CST);
		if (checked == data->threads_amount)
			break ;
		usleep(500);
	}
	__atomic_store_n(&data->threads_done, 0, __ATOMIC_SEQ_CST);
	return (0);
}

uint32_t	without_aa(t_data *data, int i, int j)
{
	t_vec3	temp_offset;
	t_vec3	pixel_sample;
	t_vec3	ray_direction;
	t_ray	ray;
	t_rgb	color;

	temp_offset = vec3_add(vec3_multiply(data->camera.pixel_delta_u, j),
			vec3_multiply(data->camera.pixel_delta_v, i));
	pixel_sample = vec3_add(data->camera.pixel00_loc, temp_offset);
	ray_direction = vec3_sub(pixel_sample, data->camera.cords);
	ray.origin = data->camera.cords;
	ray.direction = ray_direction;
	color = ray_color(&ray, data, MAX_DEPTH);
	return (rgb_to_uint32(&color));
}

void	render(t_data *data)
{
	int	i;
	int	j;
	int	render_time;

	render_time = get_time_in_ms();
	i = -1;
	if (MULTI_THREADING)
		render_with_mt(data);
	else
	{
		while (++i < data->defines.height)
		{
			j = -1;
			while (++j < data->defines.width)
			{
				if (data->settings.aa_state)
					mlx_put_pixel(data->img, j, i, monte_carlo_aa(data, i, j));
				else
					mlx_put_pixel(data->img, j, i, without_aa(data, i, j));
			}
			printf("%d\n", i);
		}
	}
	printf("\n%d\n%.2f fps\n", get_time_in_ms() - render_time, 1000
		/ (double)(get_time_in_ms() - render_time));
}

static void	render_tile_loop(t_data *data, t_tile *tile)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	pixel_x;
	uint32_t	pixel_y;

	i = 0;
	while (i < tile->height)
	{
		j = 0;
		while (j < tile->width)
		{
			pixel_x = tile->x + j;
			pixel_y = tile->y + i;
			if (data->settings.aa_state)
				data->pixels[i * tile->width + j] = monte_carlo_aa(data,
						pixel_y, pixel_x);
			else
				data->pixels[i * tile->width + j] = without_aa(data,
						pixel_y, pixel_x);
			j++;
		}
		i++;
	}
}

uint32_t	*render_tile(t_data *data, t_tile *tile)
{
	int	thread_idx;

	data->pixels = malloc(tile->height * tile->width * sizeof(uint32_t));
	if (!data->pixels)
		return (NULL);
	if (MULTI_THREADING)
	{
		thread_idx = 0;
		while (thread_idx < data->threads_amount)
		{
			data->threads[thread_idx].tile = tile;
			thread_idx++;
		}
		render_with_mt(data);
	}
	else
		render_tile_loop(data, tile);
	return (data->pixels);
}
