/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_multiply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:57:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/27 11:03:38 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

inline t_rgb	rgb_multiply(const t_rgb color, const double t)
{
	return ((t_rgb){color.r * t, color.g * t, color.b * t});
}
