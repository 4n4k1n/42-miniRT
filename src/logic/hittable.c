#include "mini_rt.h"

/**
 * Determines if ray hits front or back face of surface
 * Sets normal vector to point against ray direction
 * Used for proper lighting calculations
 */
void	set_face_normal(t_hit_record *rec, const t_ray *r, const t_vec3 *outw)
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
 * Generic hit test function for any object type
 * Dispatches to appropriate hit function based on object type
 * Currently only supports spheres, expandable for other shapes
 */
int	hittable_hit(const t_obj *o, t_ray *r, double min, double max, t_hit_record *rec)
{
	if (!o || !rec)
		return (0);
	if (o->type == SPHERE)
		return (hit_sphere_obj(&o->data.sphere, r, min, max, rec));
	if (o->type == PLANE)
		return (hit_plane_obj(&o->data.plane, r, min, max, rec));
	if (o->type == CYLINDER)
		return (hit_cylinder_obj(&o->data.cylinder, r, min, max, rec));
	if (o->type == PYRAMID)
		return (hit_pyramid_obj(&o->data.pyramid, r, min, max, rec));
	if (o->type == CONE)
		return (hit_cone_obj(&o->data.cone, r, min, max, rec));
	if (o->type == TRIANGLE)
		return (hit_triangle_obj(&o->data.triangle, r, min, max, rec));
	return (0);
}

/**
 * Tests ray intersection against all objects in the scene
 * Uses BVH if available, falls back to linear search
 * Finds closest intersection within distance range [min, max]
 * Returns 1 if any object hit, 0 otherwise
 */
int	world_hit(const t_obj_list *list, t_ray *r, double min, double max, \
		t_hit_record *out)
{
	t_hit_record	tmp;
	double			closest;
	t_obj			*cur;
	int				hit_any;

	if (!list || !out)
		return (0);
	closest = max;
	hit_any = 0;
	cur = list->head;
	while (cur)
	{
		if (hittable_hit(cur, r, min, closest, &tmp))
		{
			hit_any = 1;
			closest = tmp.t;
			*out = tmp;
		}
		cur = cur->next;
	}
	return (hit_any);
}

/**
 * Tests ray intersection using BVH acceleration structure
 * Also tests planes separately as they are excluded from BVH
 */
int	world_hit_bvh(t_bvh_node *bvh, t_obj_list *objects, t_ray *r, \
		double min, double max, t_hit_record *out)
{
	t_hit_record	temp_rec;
	t_obj			*cur;
	int				hit_any;
	double			closest;

	if (!out)
		return (0);
	hit_any = 0;
	closest = max;
	if (bvh && bvh_hit(bvh, r, min, closest, out))
	{
		hit_any = 1;
		closest = out->t;
	}
	if (objects)
	{
		cur = objects->head;
		while (cur)
		{
			if (cur->type == PLANE && hittable_hit(cur, r, min, closest, \
					&temp_rec))
			{
				hit_any = 1;
				closest = temp_rec.t;
				*out = temp_rec;
			}
			cur = cur->next;
		}
	}
	return (hit_any);
}