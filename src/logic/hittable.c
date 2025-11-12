/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 12:54:12 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Determines whether a ray hits the front or back face of a surface and
 * sets the hit record normal to point against the ray direction.
 *
 * @param rec hit record to update (front_face and normal)
 * @param r incoming ray
 * @param outw outward-facing normal computed by the geometry
 */
__attribute__((always_inline)) inline void	set_face_normal(t_hit_record *rec,
	const t_ray *r, const t_vec3 *outw)
{
	double	dotv;

	dotv = vec3_dot(r->direction, *outw);
	rec->front_face = (dotv < 0.0);
	if (rec->front_face)
		rec->normal = *outw;
	else
		rec->normal = vec3_overload(*outw);
}

/**
 * Generic hit dispatcher for all object types.
 * Calls the appropriate shape-specific hit function based on o->type.
 *
 * @param o object to test against
 * @param r ray to cast
 * @param range acceptable hit t range (tmin/tmax)
 * @param rec output hit record (filled on hit)
 * @return 1 if the object was hit, 0 otherwise
 */
int	hittable_hit(const t_obj *o, t_ray *r, t_hit_range range,
	t_hit_record *rec)
{
	if (!o || !rec)
		return (0);
	if (o->type == SPHERE)
		return (hit_sphere_obj(&o->data.sphere, r, range, rec));
	if (o->type == PLANE)
		return (hit_plane_obj(&o->data.plane, r, range, rec));
	if (o->type == CYLINDER)
		return (hit_cylinder_obj(&o->data.cylinder, r, range, rec));
	if (o->type == PYRAMID)
		return (hit_pyramid_obj(&o->data.pyramid, r, range, rec));
	if (o->type == CONE)
		return (hit_cone_obj(&o->data.cone, r, range, rec));
	if (o->type == TRIANGLE)
		return (hit_triangle_obj(&o->data.triangle, r, range, rec));
	return (0);
}

/**
 * Brute-force world hit test over an object list.
 * Iterates all objects and keeps the closest valid hit within range.
 *
 * @param list linked list of objects
 * @param r ray to cast
 * @param range hit range (tmin/tmax)
 * @param out receives closest hit record if any
 * @return 1 if any hit was found, 0 otherwise
 */
int	world_hit(const t_obj_list *list, t_ray *r, t_hit_range range,
	t_hit_record *out)
{
	t_hit_record	tmp;
	t_hit_range		cur_range;
	t_obj			*cur;
	int				hit_any;

	if (!list || !out)
		return (0);
	cur_range.tmax = range.tmax;
	hit_any = 0;
	cur = list->head;
	while (cur)
	{
		cur_range.tmin = range.tmin;
		if (hittable_hit(cur, r, cur_range, &tmp))
		{
			hit_any = 1;
			cur_range.tmax = tmp.t;
			*out = tmp;
		}
		cur = cur->next;
	}
	return (hit_any);
}

/**
 * Helper that checks plane objects in the list (planes are excluded from BVH)
 * Updates the provided range/out/hit_any when a closer plane hit is found.
 *
 * @param ctx pointer to t_plane_check context containing objects, ray, range,
 *            out record and hit_any flag
 */
static void	check_plane_hits(t_plane_check *ctx)
{
	t_hit_record	temp_rec;
	t_obj			*cur;

	cur = ctx->objects->head;
	while (cur)
	{
		if (cur->type == PLANE && hittable_hit(cur, ctx->r, *ctx->range,
				&temp_rec))
		{
			*ctx->hit_any = 1;
			ctx->range->tmax = temp_rec.t;
			*ctx->out = temp_rec;
		}
		cur = cur->next;
	}
}

/**
 * World hit test using BVH acceleration structure.
 * Queries the BVH first, then checks plane objects separately because
 * planes are kept out of the BVH. Returns the closest hit overall.
 *
 * @param bvh_ctx BVH query context (bvh root, objects list, ray, range)
 * @param out receives closest hit record if any
 * @return 1 if any hit was found, 0 otherwise
 */
int	world_hit_bvh(t_bvh_ctx *bvh_ctx, t_hit_record *out)
{
	t_plane_check	plane_ctx;
	t_hit_range		range;
	int				hit_any;

	if (!out)
		return (0);
	hit_any = 0;
	range = bvh_ctx->range;
	if (bvh_ctx->bvh && bvh_hit(bvh_ctx->bvh, bvh_ctx->r, range, out))
	{
		hit_any = 1;
		range.tmax = out->t;
	}
	if (bvh_ctx->objects)
	{
		plane_ctx = (t_plane_check){bvh_ctx->objects, bvh_ctx->r, &range,
			out, &hit_any};
		check_plane_hits(&plane_ctx);
	}
	return (hit_any);
}
