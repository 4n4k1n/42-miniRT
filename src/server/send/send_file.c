/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 12:37:54 by anakin            #+#    #+#             */
/*   Updated: 2025/10/12 14:01:59 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
