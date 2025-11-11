/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:56:32 by anakin            #+#    #+#             */
/*   Updated: 2025/10/29 14:14:20 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

__attribute__((always_inline)) inline t_rgb	rgb_add(const t_rgb a,
	const t_rgb b)
{
	return ((t_rgb){fmin(a.r + b.r, 255.999), fmin(a.g + b.g, 255.999), fmin(a.b
			+ b.b, 255.999)});
}
