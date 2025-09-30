#ifndef MINI_RT_H
# define MINI_RT_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include "objects.h"
# include "parsing.h"
# include "own_math.h"
# include "camera.h"

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"

# define WIDTH 1920
# define HEIGHT 820
# define ASPECT_RATIO WIDTH / HEIGHT
t_rgb	ray_color(t_ray *ray);
uint32_t	rgb_to_uint32(t_rgb *color);

#endif