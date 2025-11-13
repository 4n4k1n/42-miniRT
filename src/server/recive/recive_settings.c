/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_settings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:58:32 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:45:18 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Receive renderer settings from the socket.
 * Expects a MSG_SETTINGS header, reads a t_settings payload and converts all
 * integer fields from network to host byte order. Returns the populated
 * settings structure.
 * @param socket_fd connected socket descriptor
 * @return t_settings populated from the remote peer
 */
t_settings	recive_settings(int socket_fd)
{
	t_msg_header	header;
	t_settings		settings;

	header = recive_header(socket_fd);
	if (header.msg_type != MSG_SETTINGS)
		printf("Warning: unexpected message type %d\n", header.msg_type);
	recv_all(socket_fd, &settings, sizeof(t_settings));
	settings.aa_state = ntohl(settings.aa_state);
	settings.depth = ntohl(settings.depth);
	settings.light_state = ntohl(settings.light_state);
	settings.max_samples = ntohl(settings.max_samples);
	settings.min_samples = ntohl(settings.min_samples);
	settings.scale = ntohl(settings.scale);
	settings.shadow_samples = ntohl(settings.shadow_samples);
	settings.use_bvh = ntohl(settings.use_bvh);
	settings.width = ntohl(settings.width);
	settings.height = ntohl(settings.height);
	settings.aspect_ratio_int = ntohl(settings.aspect_ratio_int);
	settings.aa_max_samples = ntohl(settings.aa_max_samples);
	return (settings);
}
