#ifndef SERVER_H
# define SERVER_H

# include "mini_rt.h"

# ifndef MAX_WORKER
#  define MAX_WORKER
# endif

# define MSG_SCENE_DATA      1
# define MSG_RENDER_TILE     2
# define MSG_TILE_COMPLETE   3
# define MSG_WORKER_READY    4
# define MSG_ERROR           5
# define MSG_SCENE_FILE      6

typedef struct s_msg_header
{
    uint32_t msg_type; 
    uint32_t payload_size;
}   t_msg_header;

typedef struct s_tile
{
    uint32_t tile_id;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
}   t_tile;

typedef struct s_queue
{
    t_tile  *tiles;
    size_t  size;
    size_t  current;
    pthread_mutex_t lock;
}   t_queue;

typedef struct s_worker
{
    int     socket_fd;
    pthread_t   thread;
    uint32_t    tiles_completed;
    bool        is_active;
}   t_worker;

typedef struct s_master
{
    int socket_fd;
    t_worker    *workers;
    int         num_worker;
    t_queue     *queue;
    mlx_image_t *img;
    pthread_mutex_t img_lock;
    char            *scene_file;
    bool            shutdown;
}   t_master;

int send_file(char *path, int *worker_fds, int worker_amount);
void    send_header(int socket_fd, uint32_t msg_type, uint32_t payload);
void    send_tile_assignment(int socket_fd, t_tile *tile);
void    send_tile_result(int socket_fd, t_tile *tile_result, uint32_t *pixels);

char *recive_scene_file(int socket_fd);
t_tile  recive_tile_assignment(int socket_fd);
void    recive_tile_result(int socket_fd, t_tile *tile, uint32_t **pixels);
t_msg_header    recive_header(int socket_fd);

void    init_queue(t_queue *queue, uint32_t width, uint32_t height, uint32_t tile_size);
bool    queue_next_job(t_queue *queue, t_tile *tile);
void    destroy_queue(t_queue *queue);

#endif