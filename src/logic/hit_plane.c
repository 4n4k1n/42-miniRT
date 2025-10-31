#include "mini_rt.h"

/**
 * Tests if a ray intersects with a plane
 * Computes UV and tangent basis at the hit for bump mapping
 * Returns 1 if hit, 0 if miss
 */
int	hit_plane_obj(const t_plane *pl, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	double	den;
	double	t;
	t_vec3	outward;
	t_vec3	hp;
	t_vec3	tan;
	t_vec3	tmp;
	double	u;
	double	v;
	t_vec3	d;

	outward = vec3_normalize(pl->norm);
	den = vec3_dot(outward, r->direction);
	if (fabs(den) < 1e-8)
		return (0);
	t = vec3_dot(vec3_sub(pl->cords, r->origin), outward) / den;
	if (t <= tmin || t >= tmax)
		return (0);
	rec->t = t;
	hp = vec3_add(r->origin, vec3_multiply(r->direction, rec->t));
	rec->p = hp;
	tmp = (fabs(outward.y) < 0.999) ? (t_vec3){0.0, 1.0, 0.0} : (t_vec3){1.0, 0.0, 0.0};
	tan = vec3_normalize(vec3_cross(tmp, outward));
	rec->tangent = tan;
	rec->bitangent = vec3_cross(outward, tan);
	d = vec3_sub(hp, pl->cords);
	u = d.x * tan.x + d.y * tan.y + d.z * tan.z;
	v = d.x * rec->bitangent.x + d.y * rec->bitangent.y + d.z * rec->bitangent.z;
	u = u - floor(u);
	v = v - floor(v);
	rec->u = u;
	rec->v = v;
	rec->bump = pl->bump;
	set_face_normal(rec, r, &outward);
	rec->rgb = pl->rgb;
	rec->mat = pl->mat;
	return (1);
}
