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
# include "hittable.h"

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"

# define WIDTH 2100
# define HEIGHT 900
# define ASPECT_RATIO ((double)WIDTH / (double)HEIGHT)

// Adaptive AA configuration
// Minimum samples per pixel before checking variance
# ifndef AA_MIN_SAMPLES
#  define AA_MIN_SAMPLES 8
# endif
// Variance threshold on normalized luminance [0,1]. stddev ~= sqrt(AA_VAR_EPS)
# ifndef AA_VAR_EPS
#  define AA_VAR_EPS 0.0004 /* stddev ~= 0.02 */
# endif


t_rgb		ray_color(t_ray *ray, t_obj_list *world);
uint32_t	rgb_to_uint32(t_rgb *color);
void	init_camera(t_data *data);

#endif