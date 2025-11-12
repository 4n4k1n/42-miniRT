/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 15:24:11 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Computes an axis-aligned bounding box
 * that contains the three triangle vertices.
 * @param tri pointer to triangle (v0, v1, v2)
 * @return axis-aligned bounding box enclosing the triangle
 */
t_aabb	get_triangle_bounds(const t_triangle *tri)
{
	t_aabb	box;

	box.min.x = fmin(fmin(tri->v0.x, tri->v1.x), tri->v2.x);
	box.min.y = fmin(fmin(tri->v0.y, tri->v1.y), tri->v2.y);
	box.min.z = fmin(fmin(tri->v0.z, tri->v1.z), tri->v2.z);
	box.max.x = fmax(fmax(tri->v0.x, tri->v1.x), tri->v2.x);
	box.max.y = fmax(fmax(tri->v0.y, tri->v1.y), tri->v2.y);
	box.max.z = fmax(fmax(tri->v0.z, tri->v1.z), tri->v2.z);
	return (box);
}

/**
 * Builds an axis-aligned bounding box for a sphere.
 * The box is centered at the sphere coordinates and expanded by the radius
 * in all directions.
 * @param s pointer to sphere (cords, diameter)
 * @return axis-aligned bounding box enclosing the sphere
 */
t_aabb	get_sphere_bounds(const t_sphere *s)
{
	t_aabb	box;
	double	radius;

	radius = s->diameter * 0.5;
	box.min.x = s->cords.x - radius;
	box.min.y = s->cords.y - radius;
	box.min.z = s->cords.z - radius;
	box.max.x = s->cords.x + radius;
	box.max.y = s->cords.y + radius;
	box.max.z = s->cords.z + radius;
	return (box);
}

/**
 * Expands an AABB to include the six cardinal points at a cap center.
 * Expands the box by points center +/- (radius,0,0), center +/- (0,radius,0)
 * and center +/- (0,0,radius). Used when constructing cylinder bounds.
 * @param box pointer to AABB to expand (in/out)
 * @param cap center of the circular cap
 * @param radius cap radius
 */
static void	expand_cap_points(t_aabb *box, t_vec3 cap, double radius)
{
	*box = aabb_expand_point(*box, vec3_add(cap, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_sub(cap, vec3_init(radius, 0, 0)));
	*box = aabb_expand_point(*box, vec3_add(cap, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_sub(cap, vec3_init(0, radius, 0)));
	*box = aabb_expand_point(*box, vec3_add(cap, vec3_init(0, 0, radius)));
	*box = aabb_expand_point(*box, vec3_sub(cap, vec3_init(0, 0, radius)));
}

/**
 * Expands the provided AABB to include the top and bottom circular caps of a
 * cylinder by sampling the six cardinal points on each cap.
 * @param box pointer to AABB to expand (in/out)
 * @param cb  cylinder bounds context (axis, half_h, center, radius)
 */
static void	expand_cylinder_caps(t_aabb *box, t_cyl_bounds *cb)
{
	t_vec3	top;
	t_vec3	bottom;
	t_vec3	offset;

	offset = vec3_multiply(cb->axis, cb->half_h);
	top = vec3_add(cb->center, offset);
	bottom = vec3_sub(cb->center, offset);
	expand_cap_points(box, top, cb->radius);
	expand_cap_points(box, bottom, cb->radius);
}

/**
 * Computes an axis-aligned bounding box for a cylinder.
 * Approximates the cylinder extent by expanding to include six cardinal points
 * around each circular cap. This is cheaper than exact analytic projection
 * and sufficient for BVH purposes.
 * @param c pointer to cylinder (cords, norm, height, diameter)
 * @return axis-aligned bounding box enclosing the cylinder
 */
t_aabb	get_cylinder_bounds(const t_cylinder *c)
{
	t_aabb			box;
	t_cyl_bounds	cb;

	cb.radius = c->diameter * 0.5;
	cb.half_h = c->height * 0.5;
	cb.axis = vec3_normalize(c->norm);
	cb.center = c->cords;
	box.min = vec3_init(INFINITY, INFINITY, INFINITY);
	box.max = vec3_init(-INFINITY, -INFINITY, -INFINITY);
	expand_cylinder_caps(&box, &cb);
	return (box);
}
