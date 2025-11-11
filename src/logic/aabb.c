/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anakin <anakin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/10/28 00:00:00 by anakin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	swap_doubles(double *a, double *b)
{
	double	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

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
 * Fast ray-AABB intersection test using slab method
 * Returns 1 if ray hits box between tmin and tmax, 0 otherwise
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
