#ifndef OBJECTS_H
# define OBJECTS_H

# include "mini_rt.h"

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef struct s_rgb
{
	double	r;
	double	g;
	double	b;
}	t_rgb;

typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER
}	t_obj_type;

typedef struct s_sphere
{
	t_vec3	vec;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	vec;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	vec;
}	t_cylinder;

typedef union u_obj_data
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
}	t_obj_data;

typedef struct s_obj
{
	t_obj_type	type;
	t_obj_data	data;
}	t_obj;

typedef struct s_obj_list
{
	t_obj	*head;
	t_obj	*tail;
	size_t	size;
}	t_obj_list;

typedef struct s_camera
{
	t_vec3	vec;
}	t_camera;

typedef struct s_light
{
	t_vec3			vec;
	t_rgb			color;
	struct s_light	*next;
}	t_light;

typedef struct s_light_list
{
	t_light	*head;
	t_light	*tail;
	size_t	size;
}	t_light_list;

typedef struct t_ambiente
{
	t_rgb	color;
}	t_ambiente;

typedef struct s_main
{
	t_obj_list		*objects;
	t_camera		camera;
	t_light_list	*light_list;
	t_ambiente		ambiente;
}	t_main;

#endif