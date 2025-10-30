/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_math.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:06:10 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 13:06:11 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OWN_MATH_H
# define OWN_MATH_H

# include "objects.h"

t_vec3	vec3_add(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_sub(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_multiply(const t_vec3 a, double t);
t_vec3	vec3_divide(const t_vec3 a, const double t);
double	vec3_dot(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_cross(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_overload(const t_vec3 a);
t_vec3	vec3_cpy(const t_vec3 vec);
double	vec3_sqrt(const t_vec3 vec);
double	vec3_squared(const t_vec3 vec);
t_vec3	vec3_init(const double x, const double y, const double z);
t_vec3	vec3_reflect(const t_vec3 v, const t_vec3 n);
t_vec3	vec3_normalize(const t_vec3 v);
int		vec3_near_zero(const t_vec3 *v);

t_vec3	apply_surface_bias(t_vec3 point, t_vec3 direction, t_vec3 normal);

void	random_seed(unsigned int seed);
double	random_double(void);
double	random_double_range(double min, double max);

t_rgb	rgb_add(const t_rgb a, const t_rgb b);
t_rgb	rgb_modulate(const t_rgb a, const t_rgb b);
t_rgb	rgb_multiply(const t_rgb color, const double t);

#endif