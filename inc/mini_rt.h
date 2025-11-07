#ifndef MINI_RT_H
# define MINI_RT_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <sys/time.h>
# include <threads.h>
# include <pthread.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <netdb.h>
# include "objects.h"
# include "camera.h"
# include "parsing.h"
# include "own_math.h"
# include "hittable.h"
# include "material.h"
# include "settings.h"
# include "server.h"

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"

t_rgb		ray_color(t_ray *ray, t_data *data, int depth);
t_rgb		calculate_direct_lighting(t_data *data, const t_hit_record *rec);
t_rgb		calculate_final_color(t_rgb *final, t_ray *current_ray);
t_rgb		sample_bump_rgb(const t_bump *b, double u, double v);
void		apply_bump_mapping(t_ray_color_vars *vars);
int			russian_roulette(t_ray_color_vars *vars);
t_rgb		get_ambient_light(t_data *data, const t_hit_record *rec);
int			is_in_shadow(t_data *data, t_shadow_calc *sc);
void		add_light_sample(t_rgb *light_contrib, t_light *light,
				const t_hit_record *rec, double diffuse);
double		clamp(double x, double minv, double maxv);
void		init_aa(t_anti_aliasing *aa);
void		update_variance(t_anti_aliasing *aa);
int			should_continue(t_anti_aliasing *aa);
void		sample_pixel(t_data *data, t_anti_aliasing *aa, int i, int j);
uint32_t	finalize_color(t_anti_aliasing *aa);
uint32_t	rgb_to_uint32(t_rgb *color);
void		init_camera(t_data *data);
uint32_t	monte_carlo_aa(t_data *data, int i, int j);
uint32_t	without_aa(t_data *data, int i, int j);
t_vec3	random_on_hemisphere(t_vec3 *normal);
t_vec3	random_unit_vec3(void);
void	update_camera(t_data *data);
void	get_camera_vectors(t_data *data, t_vec3 *forward, t_vec3 *right, t_vec3 *up);
void	key_hook(mlx_key_data_t keydata, void *param);
int	get_time_in_ms(void);
void	*thread_job(void *arg);
int	render_with_mt(t_data *data);
void	cleanup_data(t_data *data);
int	init_threads(t_data *data);
int ft_error(char *msg, int error_code);
void	*thread_job_worker(void *arg);
int	init_threads_worker(t_data *data);
int	init_threads(t_data *data);
void	cleanup_data(t_data *data);
int	change_thread_state(t_data *data);
int	ft_wait(t_thread *thread);

#endif
