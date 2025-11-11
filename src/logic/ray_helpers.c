/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:15:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/07 13:15:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_rgb	sample_bump_rgb(const t_bump *b, double u, double v)
{
	uint32_t	ix;
	uint32_t	iy;
	size_t		idx;
	double		ur;
	double		vr;

	if (!b || !b->pixels || !b->width || !b->height)
		return ((t_rgb){255.0, 255.0, 255.0});
	ur = u;
	vr = v;
	while (ur < 0.0)
		ur += 1.0;
	while (vr < 0.0)
		vr += 1.0;
	if (ur > 1.0)
		ur = ur - floor(ur);
	if (vr > 1.0)
		vr = vr - floor(vr);
	ix = (uint32_t)fmin((double)(b->width - 1), floor(ur * (double)b->width));
	iy = (uint32_t)fmin((double)(b->height - 1), floor(vr * (double)b->height));
	idx = ((size_t)iy * (size_t)b->width + (size_t)ix) * 4;
	return ((t_rgb){(double)b->pixels[idx + 0], (double)b->pixels[idx + 1],
		(double)b->pixels[idx + 2]});
}

void	apply_bump_mapping(t_ray_color_vars *vars)
{
	t_bump_ctx	ctx;

	ctx = (t_bump_ctx){vars->rec.bump, vars->rec.normal, vars->rec.tangent,
		vars->rec.bitangent};
	vars->bumped = bump_perturb_from_uv(&ctx, vars->rec.u, vars->rec.v);
	vars->front = (vec3_dot(vars->current_ray.direction,
				vars->bumped) < 0.0);
	if (vars->front)
		vars->rec.normal = vars->bumped;
	else
		vars->rec.normal = vec3_multiply(vars->bumped, -1.0);
	vars->rec.front_face = vars->front;
	vars->rec.rgb = sample_bump_rgb(vars->rec.bump, vars->rec.u,
			vars->rec.v);
}

int	russian_roulette(t_ray_color_vars *vars)
{
	vars->max_throughput = fmax(fmax(vars->throughput.r,
				vars->throughput.g), vars->throughput.b);
	vars->brightness = vars->max_throughput / 255.0;
	if (vars->brightness < 0.1)
	{
		if (random_double() > vars->brightness)
			return (1);
		vars->throughput = rgb_multiply(vars->throughput, 1.0
				/ vars->brightness);
	}
	return (0);
}
