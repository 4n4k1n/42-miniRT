/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_settings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:26:08 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/13 17:53:24 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

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
	send(socket_fd, &net_settings, sizeof(t_settings), 0);
}
