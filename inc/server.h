/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:35:13 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 15:35:14 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "mini_rt.h"

# ifndef MAX_WORKER
#  define MAX_WORKER 16
# endif

# define TILE_SIZE 256

typedef enum e_msg_types
{
	MSG_SCENE_DATA,
	MSG_RENDER_TILE,
	MSG_TILE_COMPLETE,
	MSG_WORKER_READY,
	MSG_ERROR,
	MSG_SCENE_FILE,
	MSG_SHUTDOWN,
	MSG_SETTINGS,
	MSG_UPDATE
}					t_msg_types;

typedef enum e_updated_scene_type
{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	LOOK_UP,
	LOOK_DOWN,
	LOOK_LEFT,
	LOOK_RIGHT,
	UPDATE_LIGHT,
	UPDATE_RENDERING
}					t_updated_scene_type;

typedef struct s_msg_header
{
	uint32_t		msg_type;
	uint32_t		payload_size;
}					t_msg_header;

typedef struct s_tile
{
	uint32_t		tile_id;
	uint32_t		x;
	uint32_t		y;
	uint32_t		width;
	uint32_t		height;
}					t_tile;

typedef struct s_queue
{
	t_tile			*tiles;
	size_t			size;
	size_t			current;
	pthread_mutex_t	lock;
}					t_queue;

typedef struct s_worker_info
{
	int				socket_fd;
	pthread_t		thread;
	uint32_t		tiles_completed;
	bool			is_active;
}					t_worker_info;

typedef struct s_master
{
	int				socket_fd;
	bool			start_render;
	bool			restart_render;
	pthread_t		accept_thread;
	int				worker_sockets[MAX_WORKER];
	int				num_worker;
	pthread_mutex_t	workers_lock;
	pthread_mutex_t	restart_lock;
	t_queue			*queue;
	mlx_t			*mlx;
	mlx_image_t		*img;
	pthread_mutex_t	img_lock;
	char			*scene_file;
	bool			shutdown;
	t_data			*data;
}					t_master;

typedef struct s_worker_context
{
	t_master		*master;
	int				worker_socket;
}					t_worker_context;

typedef struct s_camera_update
{
	double			x;
	double			y;
	double			z;
	double			pitch;
	double			yaw;
	uint32_t		aa_state;
	uint32_t		light_state;
}					t_camera_update;

typedef struct s_update
{
	uint32_t		updated_varible;
}					t_update;

typedef struct s_run_master
{
	t_master		master;
	t_data			data;
	char			buffer[10];
	int				prev_workers;
	fd_set			readfds;
	struct timeval	tv;
	int				ret;
}					t_run_master;

typedef struct s_queue_tmp
{
	uint32_t		i;
	uint32_t		j;
	uint32_t		tiles_x;
	uint32_t		tiles_y;
	uint32_t		tile_id;
	t_queue			*queue;
	uint32_t		width;
	uint32_t		height;
	uint32_t		tile_size;
}					t_queue_tmp;

int					send_all(int socket_fd, const void *buffer, size_t length);
int					recv_all(int socket_fd, void *buffer, size_t length);
int					send_file(char *path, int socket_fd);
void				send_header(int socket_fd, uint32_t msg_type,
						uint32_t payload);
void				send_tile_assignment(int socket_fd, t_tile *tile);
void				send_tile_result(int socket_fd, t_tile *tile_result,
						uint32_t *pixels);
void				send_settings(int socket_fd, t_settings *settings);
void				send_update(int socket_fd, uint32_t update);

char				*recive_scene_file(int socket_fd);
t_tile				recive_tile_assignment(int socket_fd);
void				recive_tile_result(int socket_fd, t_tile *tile,
						uint32_t **pixels);
t_msg_header		recive_header(int socket_fd);
t_settings			recive_settings(int socket_fd);
t_update			recive_update(int socket_fd);

void				init_queue(t_queue *queue, uint32_t width, uint32_t height,
						uint32_t tile_size);
bool				queue_next_job(t_queue *queue, t_tile *tile);
void				reset_queue(t_queue *queue);
void				destroy_queue(t_queue *queue);

int					setup_listen_socket(uint32_t port);

int					setup_master(t_master *master, t_data *data,
						char *scene_file, uint32_t port);
int					run_master(char *scene_file, uint32_t port);
int					run_worker(char *master_ip, uint32_t port);

void				*worker_thread_func(void *arg);
void				*accept_worker_threads(void *arg);
void				copy_tile_to_framebuffer(mlx_image_t *image, t_tile *result,
						uint32_t *pixels);
void				broadcast_update(t_master *master, uint32_t update_value);
void				register_worker(t_master *master, int socket_fd);
void				unregister_worker(t_master *master, int socket_fd);

void				init_worker_settings(t_settings *settings);
void				*handle_worker_disconnect(t_worker_context *context,
						t_master *master);
void				process_tile_job(t_worker_context *context,
						t_master *master);
void				worker_render_loop(t_worker_context *context,
						t_master *master);
void				worker_wait_for_restart(t_master *master,
						int worker_socket);

int					connect_to_master(char *master_ip, uint32_t port);
int					setup_scene_file(int master_socket, t_data *data);
void				handle_camera_update(t_data *data,
						t_camera_update *cam_update);
int					process_tile_render(int master_socket, t_data *data,
						uint32_t *tiles_rendered);
int					handle_msg(int master_socket, t_data *data,
						t_msg_header *header, uint32_t *tiles_rendered);

char				*get_ip_address(void);
char				*get_public_ip(void);

int					init_threads_worker(t_data *data);

#endif