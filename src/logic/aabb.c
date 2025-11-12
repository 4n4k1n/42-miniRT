/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 15:25:59 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Swap two double values in-place.
 * @param a pointer to first double
 * @param b pointer to second double
 */
static void	swap_doubles(double *a, double *b)
{
	double	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * Performs slab test for a single axis of the AABB.
 * Computes intersection t0/t1 for the slab on test->axis, orders them if the
 * ray direction is negative, then clamps the running tmin/tmax interval.
 * Returns 1 when the interval remains valid, 0 if the slab rejects the ray.
 * @param box pointer to axis-aligned bounding box
 * @param r ray to test
 * @param test in/out slab test state (axis, tmin, tmax)
 * @return 1 if slab is intersected within the running interval, 0 otherwise
 */
static int	test_slab(const t_aabb *box, const t_ray *r,
	t_slab_test *test)
{
	double	inv_d;
	double	t0;
	double	t1;

	inv_d = 1.0 / ((double *)&r->direction)[test->axis];
	t0 = (((double *)&box->min)[test->axis]
			- ((double *)&r->origin)[test->axis]) * inv_d;
	t1 = (((double *)&box->max)[test->axis]
			- ((double *)&r->origin)[test->axis]) * inv_d;
	if (inv_d < 0.0)
		swap_doubles(&t0, &t1);
	test->tmin = fmax(t0, test->tmin);
	test->tmax = fmin(t1, test->tmax);
	if (test->tmax <= test->tmin)
		return (0);
	return (1);
}

/**
 * Fast ray-AABB intersection test using the slab method.
 * Tests the ray against the three axis-aligned slabs of the box. The provided
 * tmin/tmax form the initial valid interval; the function returns true if the
 * ray intersects the box within that interval.
 * @param box pointer to axis-aligned bounding box
 * @param r ray to test
 * @param tmin minimum t value (inclusive)
 * @param tmax maximum t value (exclusive)
 * @return 1 if ray intersects the box between tmin and tmax, 0 otherwise
 */
__attribute__((always_inline)) inline int	aabb_hit(const t_aabb *box,
	const t_ray *r, double tmin, double tmax)
{
	t_slab_test	test;

	test.tmin = tmin;
	test.tmax = tmax;
	test.axis = 0;
	while (test.axis < 3)
	{
		if (!test_slab(box, r, &test))
			return (0);
		test.axis++;
	}
	return (1);
}
