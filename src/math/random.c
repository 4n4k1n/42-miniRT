/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:20:10 by apregitz          #+#    #+#             */
/*   Updated: 2025/11/12 16:28:11 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static unsigned long long			g_rng_state = 88172645463393265ULL;

/**
 * Seed the internal RNG state. Passing seed == 0 leaves the state unchanged.
 * @param seed new seed value (0 means "do not reseed")
 */
void	random_seed(unsigned int seed)
{
	if (seed != 0u)
		g_rng_state = (unsigned long long)seed;
}

/* xorshift64* core: update and return raw 64-bit random value */
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

/**
 * Return a uniformly distributed double in [0,1).
 * Extracts a 53-bit mantissa from the xorshift output and scales to the
 * double precision unit interval.
 * @return pseudo-random double in [0,1)
 */
__attribute__((always_inline)) inline double	random_double(void)
{
	const unsigned long long	r = rng_next_u64();
	const unsigned long long	mantissa = r >> 11;

	return ((double)mantissa * (1.0 / 9007199254740992.0));
}

/**
 * Sample a double uniformly in [min, max).
 * @param min lower bound (inclusive)
 * @param max upper bound (exclusive)
 * @return sampled double in the specified range
 */
__attribute__((always_inline)) inline double	random_double_range(double min,
	double max)
{
	return (min + (max - min) * random_double());
}
