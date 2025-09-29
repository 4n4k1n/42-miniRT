/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:34:21 by apregitz          #+#    #+#             */
/*   Updated: 2025/09/29 19:49:22 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

inline t_vec3	vec3_init_inline(double x, double y, double z)
{
	t_vec3	new;
	
	new.x = x;
	new.y = y;
	new.z = z;
	return (new);
}
