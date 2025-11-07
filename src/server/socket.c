/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 10:54:08 by anakin            #+#    #+#             */
/*   Updated: 2025/10/30 11:20:43 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
