#ifndef SERVER_H
# define SERVER_H

# include "mini_rt.h"

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

int send_file(char *path, int *worker_fds, int worker_amount);
void    send_header(int socket_fd, uint32_t msg_type, uint32_t payload);
void    send_tile_assignment(int socket_fd, t_tile *tile);
void    send_tile_result(int socket_fd, t_tile *tile_result, uint32_t *pixels);

char *recive_scene_file(int socket_fd);
t_tile  recive_tile_assignment(int socket_fd);
void    recive_tile_result(int socket_fd, t_tile *tile, uint32_t **pixels);

#endif