/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:34:27 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 22:20:11 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include "../MLX42/include/MLX42/MLX42.h"
# include <stdint.h>

typedef struct s_material		t_material;
typedef struct s_data			t_data;
typedef struct s_tile			t_tile;

typedef struct s_defines
{
	int							width;
	int							height;
	float						aspect_ratio;
	int							aa_max_samples;
}								t_defines;

typedef struct s_vec3
{
	double						x;
	double						y;
	double						z;
}								t_vec3;

typedef struct s_ray
{
	t_vec3						origin;
	t_vec3						direction;
}								t_ray;

typedef struct s_rgb
{
	double						r;
	double						g;
	double						b;
}								t_rgb;

typedef struct s_bump
{
	unsigned char				*pixels;
	uint32_t					width;
	uint32_t					height;
	double						strength;
}								t_bump;

typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	PYRAMID,
	CONE,
	TRIANGLE
}								t_obj_type;

typedef struct s_sphere
{
	t_vec3						cords;
	double						diameter;
	t_rgb						rgb;
	t_material					*mat;
	t_bump						*bump;
}								t_sphere;

typedef struct s_plane
{
	t_vec3						cords;
	t_vec3						norm;
	t_rgb						rgb;
	t_material					*mat;
	t_bump						*bump;
}								t_plane;

typedef struct s_cylinder
{
	t_vec3						cords;
	t_vec3						norm;
	double						diameter;
	double						height;
	t_rgb						rgb;
	t_material					*mat;
	t_bump						*bump;
}								t_cylinder;

typedef struct s_cone
{
	t_vec3						cords;
	t_vec3						norm;
	double						diameter;
	double						height;
	t_rgb						rgb;
	t_material					*mat;
	t_bump						*bump;
}								t_cone;

typedef struct s_pyramid
{
	t_vec3						cords;
	t_vec3						norm;
	double						base;
	double						height;
	t_rgb						rgb;
	t_material					*mat;
	t_bump						*bump;
}								t_pyramid;

typedef struct s_triangle
{
	t_vec3						v0;
	t_vec3						v1;
	t_vec3						v2;
	t_rgb						rgb;
	t_material					*mat;
}								t_triangle;

typedef struct s_cyl_hit
{
	t_vec3						axis;
	t_vec3						k;
	t_vec3						point;
	t_vec3						v;
	t_vec3						axis_part;
	t_vec3						outward;
	double						axis_len;
	double						radius;
	double						half_h;
	double						d_dot_a;
	double						k_dot_a;
	double						a;
	double						h;
	double						c;
	double						disc;
	double						root;
	double						s;
	double						len;
}								t_cyl_hit;

typedef struct s_cyl_cap_ctx
{
	const t_cylinder			*cyl;
	t_vec3						center;
	double						t;
	int							top;
}								t_cyl_cap_ctx;

typedef struct s_cyl_best
{
	int							hit;
	double						t;
	t_vec3						p;
	t_vec3						n;
}								t_cyl_best;

typedef struct s_cyl_record_ctx
{
	const t_cylinder			*cyl;
	t_ray						*r;
	t_cyl_hit					*ch;
	t_cyl_best					*best;
}								t_cyl_record_ctx;

typedef struct s_tri_calc
{
	t_vec3						e1;
	t_vec3						e2;
	t_vec3						p;
	t_vec3						tvec;
	double						det;
	double						u;
	double						v;
}								t_tri_calc;

typedef union u_obj_data
{
	t_sphere					sphere;
	t_plane						plane;
	t_cylinder					cylinder;
	t_pyramid					pyramid;
	t_cone						cone;
	t_triangle					triangle;
}								t_obj_data;

typedef struct s_obj
{
	t_obj_type					type;
	t_obj_data					data;
	struct s_obj				*next;
}								t_obj;

typedef struct s_obj_list
{
	t_obj						*head;
	t_obj						*tail;
	size_t						size;
}								t_obj_list;

typedef struct s_aabb
{
	t_vec3						min;
	t_vec3						max;
}								t_aabb;

typedef struct s_bvh_node
{
	t_aabb						box;
	struct s_bvh_node			*left;
	struct s_bvh_node			*right;
	t_obj						**objects;
	int							obj_count;
}								t_bvh_node;

typedef struct s_light
{
	t_vec3						cords;
	t_rgb						color;
	double						intensity;
	struct s_light				*next;
}								t_light;

typedef struct s_light_list
{
	t_light						*head;
	t_light						*tail;
	size_t						size;
}								t_light_list;

typedef struct t_ambient
{
	double						lighting;
	t_rgb						color;
}								t_ambient;

typedef struct s_anti_aliasing
{
	int							min_spp;
	int							max_spp;
	double						acc_r;
	double						acc_g;
	double						acc_b;
	int							n;
	double						mean;
	double						m2;
	double						off_u;
	double						off_v;
	t_vec3						temp_u;
	t_vec3						temp_v;
	t_vec3						temp_offset;
	t_vec3						pixel_sample;
	t_vec3						ray_direction;
	t_ray						ray;
	t_rgb						sample;
	double						luma;
	double						delta;
	double						delta2;
	double						var;
	double						inv_n;
	double						avg_r;
	double						avg_g;
	double						avg_b;
	double						rbyte;
	double						gbyte;
	double						bbyte;
	t_rgb						color;
	uint32_t					packed;
}								t_anti_aliasing;

typedef struct s_hit_record		t_hit_record;
typedef struct s_ray_color_vars	t_ray_color_vars;

typedef struct s_camera
{
	double						aspect_ratio;
	int							image_width;
	int							image_height;
	t_vec3						center;
	t_vec3						pixel00_loc;
	t_vec3						pixel_delta_u;
	t_vec3						pixel_delta_v;
	t_vec3						cords;
	t_vec3						orientation;
	double						pitch;
	double						yaw;
	double						foc;
	double						viewport_height;
	double						viewport_width;
	t_vec3						viewport_u;
	t_vec3						viewport_v;
	t_vec3						viewport_upper_left;
	int							samples_per_pixel;
}								t_camera;

typedef struct s_thread
{
	size_t						id;
	pthread_t					thread;
	t_data						*data;
	bool						active;
	bool						shutdown;
	pthread_mutex_t				active_mutex;
	pthread_cond_t				active_cond;
	t_tile						*tile;
}								t_thread;

typedef struct s_settings
{
	uint32_t					scale;
	uint32_t					aa_state;
	uint32_t					light_state;
	uint32_t					min_samples;
	uint32_t					max_samples;
	uint32_t					shadow_samples;
	uint32_t					depth;
	uint32_t					use_bvh;
}								t_settings;

typedef struct s_master			t_master;

typedef struct s_data
{
	uint32_t					*pixels;
	t_settings					settings;
	t_anti_aliasing				aa;
	t_obj_list					*objects;
	t_bvh_node					*bvh_root;
	t_camera					camera;
	t_light_list				*light_list;
	t_ambient					ambiente;
	int							height;
	int							width;
	int							threads_amount;
	t_thread					*threads;
	int							threads_done;
	mlx_t						*mlx;
	mlx_image_t					*img;
	t_master					*master;
	t_defines					defines;
}								t_data;

#endif