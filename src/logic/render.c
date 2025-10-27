/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:31:15 by anakin            #+#    #+#             */
/*   Updated: 2025/10/27 10:33:45 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

uint32_t	without_aa(t_data *data, int i, int j)
{
	t_vec3	temp_u;
	t_vec3	temp_v;
	t_vec3	temp_offset;
	t_vec3	pixel_sample;
	t_vec3	ray_direction;
	t_ray	ray;
	t_rgb	color;

	temp_u = vec3_multiply(data->camera.pixel_delta_u, j);
	temp_v = vec3_multiply(data->camera.pixel_delta_v, i);
	temp_offset = vec3_add(temp_u, temp_v);
	pixel_sample = vec3_add(data->camera.pixel00_loc, temp_offset);
	ray_direction = vec3_sub(pixel_sample, data->camera.cords);
	ray.origin = data->camera.cords;
	ray.direction = ray_direction;
	color = ray_color(&ray, data, MAX_DEPTH);
	return (rgb_to_uint32(&color));
}

void	render(t_data *data)
{
	int		i;
	int		j;
	int	render_time;

	render_time = get_time_in_ms();
	i = 0;
	if (MULTI_THREADING)
		render_with_mt(data);
	else
	{
		while (i < HEIGHT)
		{
			j = 0;
			while (j < WIDTH)
			{
				if (data->settings.aa_state)
					mlx_put_pixel(data->img, j, i, monte_carlo_aa(data, i, j));
				else
					mlx_put_pixel(data->img, j, i, without_aa(data, i, j));
				j++;
			}
			printf("%d\n", i);
			i++;
		}
	}
	printf("\n%d\n%.2f fps\n", get_time_in_ms() - render_time, 1000 / (double)(get_time_in_ms() - render_time));
}

uint32_t *render_tile(t_data *data, t_tile *tile)
{
	uint32_t    i;
	uint32_t    j;
	uint32_t	pixel_x;
	uint32_t	pixel_y;
	int			thread_idx;

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
	{
		i = 0;
		while (i < tile->height)
		{
			j = 0;
			while (j < tile->width)
			{
				pixel_x = tile->x + j;
				pixel_y = tile->y + i;

				if (data->settings.aa_state)
					data->pixels[i * tile->width + j] = monte_carlo_aa(data, pixel_y, pixel_x);
				else
					data->pixels[i * tile->width + j] = without_aa(data, pixel_y, pixel_x);
				j++;
			}
			i++;
		}
	}
	return (data->pixels);
}
