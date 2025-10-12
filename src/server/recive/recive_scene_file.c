/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_scene_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:07:35 by anakin            #+#    #+#             */
/*   Updated: 2025/10/11 22:14:58 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

char *recive_scene_file(int socket_fd)
{
    t_msg_header    header;
    char *contents;

    header = recive_header(socket_fd);
    if (header.msg_type != MSG_SCENE_FILE)
        return (NULL);
    contents = malloc(header.payload_size + 1);
    if (!contents)
        return (ft_error("malloc", 1), NULL);
    recv(socket_fd, contents, header.payload_size, MSG_WAITALL);
    contents[header.payload_size] = '\0';
    return (contents);
}
