/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:33:02 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 15:33:54 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

struct							s_ray;
struct							s_rgb;
struct							s_hit_record;
struct							s_scatter_ctx;

typedef struct s_material		t_material;

typedef int						(*t_scatter_fn)(const t_material *self,
							struct s_scatter_ctx *ctx);

typedef enum e_material_type
{
	LAMBERTIAN,
	METAL,
	GLASS,
	PHONG
}								t_material_type;

typedef enum e_texture_type
{
	NONE,
	CHECKER,
}								t_texture_type;

struct							s_scatter_ctx
{
	const struct s_ray			*r_in;
	const struct s_hit_record	*rec;
	struct s_rgb				*attenuation;
	struct s_ray				*scattered;
};

struct							s_material
{
	t_scatter_fn				scatter;
	struct s_rgb				albedo;
	double						fuzz;
	int							type;
	double						refraction_index;
	int							texture_type;
	struct s_rgb				texture_a;
	struct s_rgb				texture_b;
	double						texture_scale;
	double						shininess;
	struct s_rgb				specular;
};

t_material						*material_lambertian(struct s_rgb albedo);
t_material						*material_metal(struct s_rgb albedo,
									double fuzz);
t_material						*material_dielectric(double refraction_index);
t_material						*material_phong(struct s_rgb albedo,
									double shininess);

double							get_refraction_index(const t_material *self,
									int front_face);
double							get_sin_theta(double cos_theta);
struct s_rgb					get_texture_color(const t_material *self,
									double u, double v);

#endif