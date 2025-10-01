#ifndef MINI_RT_H
# define MINI_RT_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include "objects.h"
# include "camera.h"
# include "parsing.h"
# include "own_math.h"
# include "hittable.h"

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"

# define WIDTH 1920
# define HEIGHT 1080
# define ASPECT_RATIO ((double)WIDTH / (double)HEIGHT)

// Adaptive AA configuration
// Minimum samples per pixel before checking variance
# ifndef AA_MIN_SAMPLES
#  define AA_MIN_SAMPLES 32
# endif

# ifndef AA_MAX_SAMPLES
#  define AA_MAX_SAMPLES 128
# endif

// Variance threshold on normalized luminance [0,1]. stddev ~= sqrt(AA_VAR_EPS)
# ifndef AA_VAR_EPS
#  define AA_VAR_EPS 0.0004 /* stddev ~= 0.02 */
# endif

# define MAX_DEPTH 1024

t_rgb		ray_color(t_ray *ray, t_obj_list *world, int depth);
uint32_t	rgb_to_uint32(t_rgb *color);
void	init_camera(t_data *data);
uint32_t	monte_carlo_aa(t_data *data, t_anti_aliasing *aa, int i, int j);
uint32_t	without_aa(t_data *data, int i, int j);
t_vec3	random_on_hemisphere(t_vec3 *normal);

#endif
