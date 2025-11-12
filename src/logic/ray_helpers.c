/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:15:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:47:19 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Samples bump texture at wrapped UV coordinates.
 * Wraps u,v into [0,1) and returns the RGB at that pixel.
 * Falls back to white if texture is missing.
 * @param b bump map
 * @param u horizontal UV
 * @param v vertical UV
 * @return sampled RGB color
 */
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

/**
 * Applies bump mapping to the hit record in vars.
 * Perturbs the normal from bump map and orients it to face the ray.
 * Also replaces surface rgb with the sampled bump color at (u,v).
 * @param vars ray-color state (reads rec, writes rec.normal/front_face/rgb)
 */
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

/**
 * Russian roulette termination for low-throughput paths.
 * When brightness < 0.1, randomly terminates the path and rescales throughput.
 * @param vars ray-color state (uses/updates throughput)
 * @return 1 if terminated, 0 to continue
 */
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
