/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 13:36:56 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Returns the appropriate refraction ratio depending on whether the hit was
 * on the front face.
 * If front_face is true, returns 1.0 / material->refraction_index,
 * otherwise returns material->refraction_index.
 * @param self material descriptor (contains refraction_index)
 * @param front_face non-zero if ray hit the front face
 * @return refraction index ratio to use for refraction calculations
 */
__attribute__((always_inline)) inline double	get_refraction_index(
	const t_material *self, int front_face)
{
	if (front_face)
		return (1.0 / self->refraction_index);
	else
		return (self->refraction_index);
}

/**
 * Computes sin(theta) from cos(theta) robustly, returning 0 if numerical
 * errors produce a negative value under the sqrt.
 * @param cos_theta cosine of the incident angle (clamped expected)
 * @return sin(theta) non-negative
 */
__attribute__((always_inline)) inline double	get_sin_theta(double cos_theta)
{
	double	sin_theta_sq;

	sin_theta_sq = 1.0 - cos_theta * cos_theta;
	if (sin_theta_sq > 0.0)
		return (sqrt(sin_theta_sq));
	else
		return (0.0);
}

/**
 * Returns the checker texture color sampled at (u,v) for the given material.
 * Uses texture_scale to control checker frequency; falls back to texture_a/b.
 * @param self material containing checker colors and scale
 * @param u horizontal texture coordinate
 * @param v vertical texture coordinate
 * @return sampled checker t_rgb
 */
static t_rgb	get_checker_color(const t_material *self, double u, double v)
{
	double	s;
	int		m;

	if (self->texture_scale <= 0.0)
		s = 1.0;
	else
		s = self->texture_scale;
	m = (((int)floor(u * s)) + ((int)floor(v * s))) & 1;
	if (m)
		return (self->texture_b);
	else
		return (self->texture_a);
}

/**
 * Dispatches material texture lookup. Currently supports CHECKER and a
 * default white value when no texture is present.
 * @param self material descriptor
 * @param u horizontal texture coordinate
 * @param v vertical texture coordinate
 * @return sampled texture color as t_rgb
 */
__attribute__((always_inline)) inline t_rgb	get_texture_color(
	const t_material *self, double u, double v)
{
	if (self->texture_type == CHECKER)
		return (get_checker_color(self, u, v));
	else
		return ((t_rgb){255.0, 255.0, 255.0});
}
