/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:05:54 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 13:12:47 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

struct						s_ray;
struct						s_rgb;
struct						s_hit_record;

typedef struct s_material	t_material;

typedef int					(*t_scatter_fn)(const t_material *self,
						const struct s_ray *r_in,
						const struct s_hit_record *rec,
						struct s_rgb *attenuation, struct s_ray *scattered);

typedef enum e_material_type
{
	LAMBERTIAN,
	METAL,
	GLASS
}							t_material_type;

struct						s_material
{
	t_scatter_fn			scatter;
	struct s_rgb			albedo;
	double					fuzz;
	int						type;
	double					refraction_index;
};

t_material					*material_lambertian(struct s_rgb albedo);
t_material					*material_metal(struct s_rgb albedo, double fuzz);
t_material					*material_dielectric(double refraction_index);

#endif