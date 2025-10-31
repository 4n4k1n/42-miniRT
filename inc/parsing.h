#ifndef PARSING_H
# define PARSING_H

# include "objects.h"

typedef struct s_arg_check
{
	int	has_a;
	int	has_c;
	int	has_l;
}		t_arg_check;

// Error
int		rt_error(const char *msg);
void	free_scene(t_data *scene);

// Parsing
int		parse_material(char **tokens, int len, t_obj *o);
int		parse_scene(const char *path, t_data *scene);
int		parse_ambient(char **tokens, t_data *scene, t_arg_check *args);
int		parse_camera(char **tokens, t_data *scene, t_arg_check *args);
int		parse_light(char **tokens, t_data *scene, t_arg_check *args);
int		parse_cylinder(char **tokens, t_data *scene);
int		parse_plane(char **tokens, t_data *scene);
int		parse_sphere(char **tokens, t_data *scene);
int		parse_pyramid(char **tokens, t_data *scene);
int		parse_cone(char **tokens, t_data *scene);
int		parse_bump(const char *token, t_obj *o);
int		parse_triangle(char **tokens, t_data *scene);

// Utils
int		is_rt_file(const char *path);
int		parse_double(const char *str, double *out);
int		parse_vec3(const char *str, t_vec3 *out);
int		parse_rgb(const char *str, t_rgb *out);

// Validators
int		validate_fov(double fov);
int		vec_in_range(t_vec3 *vec, double min, double max);
int		in_range_d(double v, double min, double max);
int		vec_non_zero(t_vec3 v);

// List
t_obj	*obj_new(t_obj_type type);
int		obj_push(t_obj_list *lst, t_obj *node);
t_light	*light_new(void);
int		light_push(t_light_list *lst, t_light *node);

/* Debug printer (tester) */
void	print_scene(const t_data *scene);
void	print_vec3(const char *name, t_vec3 v);
void	print_rgb(const char *name, t_rgb c);

#endif