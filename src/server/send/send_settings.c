/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_settings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:26:08 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 17:17:23 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Send renderer settings to a connected peer.
 * Converts each integer field to network byte order and transmits a MSG_SETTINGS
 * header followed by the serialized payload.
 * @param socket_fd destination socket
 * @param settings pointer to local settings (host order)
 */
void	send_settings(int socket_fd, t_settings *settings)
{
	t_settings	net_settings;

	send_header(socket_fd, MSG_SETTINGS, sizeof(t_settings));
	net_settings.aa_state = htonl(settings->aa_state);
	net_settings.depth = htonl(settings->depth);
	net_settings.light_state = htonl(settings->light_state);
	net_settings.max_samples = htonl(settings->max_samples);
	net_settings.min_samples = htonl(settings->min_samples);
	net_settings.scale = htonl(settings->scale);
	net_settings.shadow_samples = htonl(net_settings.shadow_samples);
	net_settings.use_bvh = htonl(settings->use_bvh);
	send_all(socket_fd, &net_settings, sizeof(t_settings));
}
