#ifndef OBJECTS_H
# define OBJECTS_H

typedef struct s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

typedef struct s_rgb
{
	double			r;
	double			g;
	double			b;
}					t_rgb;

typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER
}					t_obj_type;

typedef struct s_sphere
{
	t_vec3			cords;
	double			diameter;
	t_rgb			rgb;
}					t_sphere;

typedef struct s_plane
{
	t_vec3			cords;
	t_vec3			norm;
	t_rgb			rgb;
}					t_plane;

typedef struct s_cylinder
{
	t_vec3			cords;
	t_vec3			norm;
	double			diameter;
	double			height;
	t_rgb			rgb;
}					t_cylinder;

typedef union u_obj_data
{
	t_sphere		sphere;
	t_plane			plane;
	t_cylinder		cylinder;
}					t_obj_data;

typedef struct s_obj
{
	t_obj_type		type;
	t_obj_data		data;
	struct s_obj	*next;
}					t_obj;

typedef struct s_obj_list
{
	t_obj			*head;
	t_obj			*tail;
	size_t			size;
}					t_obj_list;

typedef struct s_camera
{
	t_vec3	center;
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

typedef struct s_light
{
	t_vec3			cords;
	t_rgb			color;
	struct s_light	*next;
}					t_light;

typedef struct s_light_list
{
	t_light			*head;
	t_light			*tail;
	size_t			size;
}					t_light_list;

typedef struct t_ambient
{
	double			lighting;
	t_rgb			color;
}					t_ambient;

typedef struct s_data
{
	t_obj_list		*objects;
	t_camera		camera;
	t_light_list	*light_list;
	t_ambient		ambiente;
	int				height;
	int				width;
}	t_data;

#endif