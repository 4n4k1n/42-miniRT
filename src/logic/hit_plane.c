#include "mini_rt.h"

int	hit_plane_obj(const t_plane *pl, t_ray *r, double tmin, double tmax, t_hit_record *rec)
{
	t_vec3		n;
	double		denom;
	t_vec3		p0l0;
	double		t;
	t_vec3		tmp;

	if (!pl || !rec || !r)
		return (0);
	n = vec3_normalize(pl->norm);
	denom = vec3_dot(r->direction, n);
	if (fabs(denom) < 1e-8)
		return (0);
	p0l0 = vec3_sub(pl->cords, r->origin);
	t = vec3_dot(p0l0, n) / denom;
	if (t <= tmin || t >= tmax)
		return (0);
	rec->t = t;
	tmp = vec3_multiply(r->direction, rec->t);
	rec->p = vec3_add(r->origin, tmp);
	set_face_normal(rec, r, &n);
	rec->rgb = pl->rgb;
	rec->mat = pl->mat;
	return (1);
}
