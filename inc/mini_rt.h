#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include "objects.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include "math.h"

# define WIDTH 600
# define HEIGHT 400
# define ASPECT_RATIO 16.0 / 9.0

t_rgb	ray_color(t_ray *ray);
uint32_t	rgb_to_uint32(t_rgb *color);
void	init_camera(t_data *data);

#endif