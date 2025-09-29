/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:53:09 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 17:57:55 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	get_image_height(int width, double ra)
{
	int	height;

	height = width / ra;
	if (height < 1)
		return (1);
	return (height);
}
