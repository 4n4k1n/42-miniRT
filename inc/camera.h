#ifndef CAMERA_H
# define CAMERA_H

#include "mini_rt.h"
# include "objects.h"

typedef struct s_vec3;

typedef struct s_camera
{
	t_vec3	cords;
	t_vec3	orientation;
	double	foc;
	double	viewport_height;
	double	viewport_width;
	t_vec3	viewport_u;
	t_vec3	viewport_v;
	t_vec3	pixel_delta_u;
	t_vec3	pixel_delta_v;
	t_vec3	viewport_upper_left;
	t_vec3	pixel00_loc;
}	t_camera;

typedef struct s_pixel_tmp
{
	t_vec3  u;
	t_vec3  v;
	t_vec3  offset;
	t_vec3  center;
	t_vec3  dir;
	t_ray   ray;
}	t_pixel_tmp;

typedef struct s_init_tmp
{
	t_vec3  t1;
	t_vec3  t2;
	t_vec3  t3;
	t_vec3  t4;
	t_vec3  t5;
	t_vec3  t6;
	t_vec3  t7;
}	t_init_tmp;

void	init_camera(t_data *data);
double	hit_sphere(t_vec3 center, double radius, t_ray *r);
void	render(t_data *data);

#endif