/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 13:05:35 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 13:05:36 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "objects.h"

typedef struct s_tile	t_tile;

typedef struct s_pixel_tmp
{
	t_vec3				u;
	t_vec3				v;
	t_vec3				offset;
	t_vec3				center;
	t_vec3				dir;
	t_ray				ray;
}						t_pixel_tmp;

typedef struct s_init_tmp
{
	t_vec3				t1;
	t_vec3				t2;
	t_vec3				t3;
	t_vec3				t4;
	t_vec3				t5;
	t_vec3				t6;
	t_vec3				t7;
}						t_init_tmp;

void					init_camera(t_data *data);
double					hit_sphere(t_vec3 center, double radius, t_ray *r);
void					render(t_data *data);
uint32_t				*render_tile(t_data *data, t_tile *tile);

#endif