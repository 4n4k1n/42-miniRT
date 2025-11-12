/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 10:54:08 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 17:06:48 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Create, bind and listen on a TCP socket on the specified port.
 * Sets SO_REUSEADDR on the socket and starts listening with a backlog.
 * Returns the listening socket file descriptor on success, or a negative
 * value on failure.
 * @param port TCP port to bind to (host order)
 * @return listening socket fd or negative on error
 */
int	setup_listen_socket(uint32_t port)
{
	int					socket_fd;
	int					opt;
	struct sockaddr_in	addr;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		return (ft_error("socket", 1));
	opt = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return (ft_error("bind", 1));
	if (listen(socket_fd, 128) < 0)
		return (ft_error("listen", 1));
	printf("Master listen on port %d\n", (int)port);
	return (socket_fd);
}
