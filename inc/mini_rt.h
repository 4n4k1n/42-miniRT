#ifndef MINI_RT_H
# define MINI_RT_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <sys/time.h>
# include "objects.h"
# include "camera.h"
# include "parsing.h"
# include "own_math.h"
# include "hittable.h"
# include "material.h"
# include "settings.h"

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"

t_rgb		ray_color(t_ray *ray, t_obj_list *world, int depth);
uint32_t	rgb_to_uint32(t_rgb *color);
void	init_camera(t_data *data);
uint32_t	monte_carlo_aa(t_data *data, t_anti_aliasing *aa, int i, int j);
uint32_t	without_aa(t_data *data, int i, int j);
t_vec3	random_on_hemisphere(t_vec3 *normal);
void	update_camera(t_data *data);
void	get_camera_vectors(t_data *data, t_vec3 *forward, t_vec3 *right, t_vec3 *up);
void	key_hook(mlx_key_data_t keydata, void *param);
int	get_time_in_ms(void);

#endif
