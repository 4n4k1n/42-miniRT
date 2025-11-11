/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

__attribute__((always_inline)) inline double	get_refraction_index(
	const t_material *self, int front_face)
{
	if (front_face)
		return (1.0 / self->refraction_index);
	else
		return (self->refraction_index);
}

__attribute__((always_inline)) inline double	get_sin_theta(double cos_theta)
{
	double	sin_theta_sq;

	sin_theta_sq = 1.0 - cos_theta * cos_theta;
	if (sin_theta_sq > 0.0)
		return (sqrt(sin_theta_sq));
	else
		return (0.0);
}

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

__attribute__((always_inline)) inline t_rgb	get_texture_color(
	const t_material *self, double u, double v)
{
	if (self->texture_type == CHECKER)
		return (get_checker_color(self, u, v));
	else
		return ((t_rgb){255.0, 255.0, 255.0});
}
