/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:30 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/27 11:39:22 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	get_camera_vectors(t_data *data, t_vec3 *forward,
	t_vec3 *right, t_vec3 *up)
{
	forward->x = cos(data->camera.pitch) * cos(data->camera.yaw);
	forward->y = sin(data->camera.pitch);
	forward->z = cos(data->camera.pitch) * sin(data->camera.yaw);
	*up = vec3_init(0.0, 1.0, 0.0);
	*right = vec3_normalize(vec3_cross(*forward, *up));
	*up = vec3_normalize(vec3_cross(*right, *forward));
}

void	update_camera(t_data *data)
{
	t_init_tmp	tmp;
	t_vec3		forward;
	t_vec3		right;
	t_vec3		up;

	get_camera_vectors(data, &forward, &right, &up);
	data->camera.viewport_u = vec3_multiply(right, data->camera.viewport_width);
	data->camera.viewport_v = vec3_multiply(up, -data->camera.viewport_height);
	data->camera.pixel_delta_u = vec3_divide(data->camera.viewport_u, WIDTH);
	data->camera.pixel_delta_v = vec3_divide(data->camera.viewport_v, HEIGHT);
	tmp.t1 = vec3_multiply(forward, data->camera.foc);
	tmp.t2 = vec3_sub(data->camera.cords, tmp.t1);
	tmp.t3 = vec3_divide(data->camera.viewport_u, 2.0);
	tmp.t4 = vec3_sub(tmp.t2, tmp.t3);
	tmp.t5 = vec3_divide(data->camera.viewport_v, 2.0);
	data->camera.viewport_upper_left = vec3_sub(tmp.t4, tmp.t5);
	tmp.t6 = vec3_add(data->camera.pixel_delta_u, data->camera.pixel_delta_v);
	tmp.t7 = vec3_multiply(tmp.t6, 0.5);
	data->camera.pixel00_loc = vec3_add(data->camera.viewport_upper_left,
			tmp.t7);
}

/**
 * Calculates image height based on width and aspect ratio
 * Formula: height = width / aspect_ratio
 * Ensures minimum height of 1 pixel
 */
static int	get_image_height(int width, double ra)
{
	int	height;

	height = width / ra;
	if (height < 1)
		return (1);
	return (height);
}

/**
 * Initializes camera parameters for ray tracing
 * Sets up viewport dimensions, pixel deltas, and starting pixel location
 * Creates coordinate system for converting screen pixels to world rays
 * Uses parsed camera data from scene file (position, orientation, FOV)
 */
void	init_camera(t_data *data)
{
	double	theta;
	double	h;
	double	focal_length;
	t_vec3	normalized_dir;

	data->width = WIDTH;
	data->height = get_image_height(WIDTH, ASPECT_RATIO);
	normalized_dir = vec3_normalize(data->camera.orientation);
	data->camera.pitch = asin(normalized_dir.y);
	data->camera.yaw = atan2(normalized_dir.z, normalized_dir.x);
	focal_length = 1.0;
	theta = data->camera.foc * (M_PI / 180.0);
	h = tan(theta / 2.0);
	data->camera.viewport_height = 2.0 * h * focal_length;
	data->camera.viewport_width = data->camera.viewport_height * ASPECT_RATIO;
	data->camera.foc = focal_length;
	update_camera(data);
}
