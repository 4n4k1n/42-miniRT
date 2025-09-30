#ifndef HITTABLE_H
# define HITTABLE_H

# include "mini_rt.h"

typedef struct	s_hit_record
{
	t_vec3	p;
	t_vec3	normal;
	double	t;
	int		front_face;
	t_rgb	rgb;
}			t_hit_record;

int		hit_sphere_obj(const t_sphere *s, t_ray *r, double tmin, double tmax, t_hit_record *rec);
void	set_face_normal(t_hit_record *rec, const t_ray *r, const t_vec3 *outw);
int		hittable_hit(const t_obj *o, t_ray *r, double min, double max, t_hit_record *rec);
int		world_hit(const t_obj_list *list, t_ray *r, double min, double max, t_hit_record *out);

#endif