/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:20:10 by apregitz          #+#    #+#             */
/*   Updated: 2025/10/29 14:26:01 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static unsigned long long			g_rng_state = 88172645463393265ULL;

void	random_seed(unsigned int seed)
{
	if (seed != 0u)
		g_rng_state = (unsigned long long)seed;
}

/* xorshift64* core */
static inline unsigned long long	rng_next_u64(void)
{
	unsigned long long	x;

	x = g_rng_state;
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	g_rng_state = x;
	return (x * 2685821657736338717ULL);
}

inline double	random_double(void)
{
	const unsigned long long	r = rng_next_u64();
	const unsigned long long	mantissa = r >> 11;

	return ((double)mantissa * (1.0 / 9007199254740992.0));
}

inline double	random_double_range(double min, double max)
{
	return (min + (max - min) * random_double());
}
