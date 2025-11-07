/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recive_settings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:58:32 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/13 18:27:27 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
	return (settings);
}
