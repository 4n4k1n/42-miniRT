#include "mini_rt.h"

void	set_face_normal(t_hit_record *rec, const t_ray *r, const t_vec3 *outw)
{
	double	dotv;

	dotv = vec3_dot_inline((t_vec3 *)&r->direction, (t_vec3 *)outw);
	rec->front_face = (dotv < 0.0);
	if (rec->front_face)
		rec->normal = *outw;
	else
		rec->normal = vec3_overload_inline((t_vec3 *)outw);
}

int	hittable_hit(const t_obj *o, t_ray *r, double min, double max, t_hit_record *rec)
{
	if (!o || !rec)
		return (0);
	if (o->type == SPHERE)
		return (hit_sphere_obj(&o->data.sphere, r, min, max, rec));
	// ADD THE REST OF THE OBJS
	return (0);
}

int	world_hit(const t_obj_list *list, t_ray *r, double min, double max, t_hit_record *out)
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