/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_scene_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:07:35 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 16:45:30 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Receive a scene file payload from the socket.
 * Expects a MSG_SCENE_FILE message header containing the payload size,
 * allocates a buffer large enough for the file contents plus a terminating
 * null byte, reads the payload, null-terminates it and returns the pointer.
 * On allocation failure returns NULL after printing an error.
 * Caller is responsible for freeing the returned buffer.
 * @param socket_fd connected socket descriptor
 * @return pointer to malloc'd null-terminated scene file contents, or NULL
 */
char	*recive_scene_file(int socket_fd)
{
	t_msg_header	header;
	char			*contents;

	header = recive_header(socket_fd);
	if (header.msg_type != MSG_SCENE_FILE)
		return (NULL);
	contents = malloc(header.payload_size + 1);
	if (!contents)
		return (ft_error("malloc", 1), NULL);
	recv_all(socket_fd, contents, header.payload_size);
	contents[header.payload_size] = '\0';
	return (contents);
}
