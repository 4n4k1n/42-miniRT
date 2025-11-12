/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 12:37:54 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:17:04 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
 * Read an entire file into a malloc'd buffer. On success returns pointer and
 * sets file_size. On error returns NULL and prints a message.
 */
static char	*read_file(char *path, size_t *file_size)
{
	int			fd;
	struct stat	st;
	char		*file_content;
	size_t		bytes;

	fd = open(path, O_RDONLY, 0777);
	if (fd < 0)
		return (ft_error("open", 1), NULL);
	if (fstat(fd, &st))
		return (close(fd), ft_error("fstat", 1), NULL);
	*file_size = st.st_size;
	file_content = malloc(*file_size + 1);
	if (!file_content)
		return (close(fd), ft_error("malloc", 1), NULL);
	bytes = read(fd, file_content, *file_size);
	file_content[bytes] = '\0';
	close(fd);
	return (file_content);
}

/**
 * Send the contents of a file over socket preceded by MSG_SCENE_FILE header.
 * Reads the file into memory, sends a header with payload size and then the
 * file bytes (including terminating NUL). Returns 0 on success, 1 on error.
 * @param path filesystem path to the file
 * @param socket_fd connected socket descriptor
 * @return 0 on success, 1 on failure
 */
int	send_file(char *path, int socket_fd)
{
	char			*file_content;
	size_t			size;
	t_msg_header	header;

	file_content = read_file(path, &size);
	if (!file_content)
		return (1);
	header.msg_type = htonl(MSG_SCENE_FILE);
	header.payload_size = htonl(size + 1);
	send_all(socket_fd, &header, sizeof(header));
	send_all(socket_fd, file_content, size + 1);
	free(file_content);
	return (0);
}
