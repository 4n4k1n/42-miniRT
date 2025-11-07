/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monte_carlo_aa_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:26:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/07 13:26:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

double	clamp(double x, double minv, double maxv)
{
	if (x < minv)
		return (minv);
	if (x > maxv)
		return (maxv);
	return (x);
}

void	init_aa(t_anti_aliasing *aa)
{
	ft_memset(aa, 0, sizeof(t_anti_aliasing));
	aa->min_spp = AA_MIN_SAMPLES;
	aa->max_spp = AA_MAX_SAMPLES;
}

void	update_variance(t_anti_aliasing *aa)
{
	aa->luma = (0.2126 * aa->sample.r + 0.7152 * aa->sample.g
			+ 0.0722 * aa->sample.b) / 255.999;
	aa->n++;
	aa->delta = aa->luma - aa->mean;
	aa->mean += aa->delta / aa->n;
	aa->delta2 = aa->luma - aa->mean;
	aa->m2 += aa->delta * aa->delta2;
}

int	should_continue(t_anti_aliasing *aa)
{
	if (aa->n >= aa->min_spp)
	{
		if (aa->n > 1)
			aa->var = aa->m2 / (aa->n - 1);
		else
			aa->var = 0.0;
		if (aa->var <= AA_VAR_EPS)
			return (0);
	}
	return (1);
}

void	sample_pixel(t_data *data, t_anti_aliasing *aa, int i, int j)
{
	aa->off_u = random_double() - 0.5;
	aa->off_v = random_double() - 0.5;
	aa->temp_u = vec3_multiply(data->camera.pixel_delta_u, j + aa->off_u);
	aa->temp_v = vec3_multiply(data->camera.pixel_delta_v, i + aa->off_v);
	aa->temp_offset = vec3_add(aa->temp_u, aa->temp_v);
	aa->pixel_sample = vec3_add(data->camera.pixel00_loc, aa->temp_offset);
	aa->ray_direction = vec3_sub(aa->pixel_sample, data->camera.cords);
	aa->ray.origin = data->camera.cords;
	aa->ray.direction = aa->ray_direction;
	aa->sample = ray_color(&aa->ray, data, MAX_DEPTH);
	aa->acc_r += aa->sample.r;
	aa->acc_g += aa->sample.g;
	aa->acc_b += aa->sample.b;
}
