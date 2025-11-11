/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_modulate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:56:46 by anakin            #+#    #+#             */
/*   Updated: 2025/10/29 14:15:18 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

__attribute__((always_inline)) inline t_rgb	rgb_modulate(const t_rgb a,
	const t_rgb b)
{
	return ((t_rgb){(a.r * b.r) / 255.999, (a.g * b.g) / 255.999, (a.b * b.b)
		/ 255.999});
}
