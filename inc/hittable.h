/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:32:45 by nweber            #+#    #+#             */
/*   Updated: 2025/11/11 15:57:25 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HITTABLE_H
# define HITTABLE_H

# include "mini_rt.h"

typedef struct s_hit_range
{
	double	tmin;
	double	tmax;
}	t_hit_range;

typedef struct s_slab_test
{
	double	tmin;
	double	tmax;
	int		axis;
}	t_slab_test;

typedef struct s_cyl_bounds
{
	t_vec3	center;
	t_vec3	axis;
	double	radius;
	double	half_h;
}	t_cyl_bounds;

typedef struct s_plane_basis
{
	t_vec3	c;
	t_vec3	t;
	t_vec3	b;
	double	hs;
}	t_plane_basis;

typedef struct s_sphere_hit
{
	t_vec3		oc;
	double		radius;
	t_hit_range	range;
}	t_sphere_hit;

typedef struct s_tri_hit_ctx
{
	t_hit_range	range;
	t_tri_calc	calc;
}	t_tri_hit_ctx;

typedef struct s_py_tri_calc
{
	t_vec3	e1;
	t_vec3	e2;
	t_vec3	p;
	double	det;
	double	inv_det;
}	t_py_tri_calc;

typedef struct s_py_tri_uv
{
	t_vec3	tvec;
	t_vec3	q;
	double	u;
	double	v;
}	t_py_tri_uv;

typedef struct s_py_verts
{
	t_vec3	apex;
	t_vec3	b0;
	t_vec3	b1;
	t_vec3	b2;
	t_vec3	b3;
}	t_py_verts;

typedef struct s_py_best
{
	int		hit;
	double	t;
	t_vec3	n;
}	t_py_best;

typedef struct s_py_basis
{
	t_vec3	bc;
	t_vec3	r;
	t_vec3	f;
	double	hw;
}	t_py_basis;

typedef struct s_plane_check
{
	t_obj_list		*objects;
	t_ray			*r;
	t_hit_range		*range;
	t_hit_record	*out;
	int				*hit_any;
}	t_plane_check;

typedef struct s_bvh_ctx
{
	t_bvh_node		*bvh;
	t_obj_list		*objects;
	t_ray			*r;
	t_hit_range		range;
}	t_bvh_ctx;

typedef struct s_cone_calc
{
	t_vec3	a;
	t_vec3	apex;
	t_vec3	v;
	double	hh;
	double	rad;
	double	m;
	double	c2;
}	t_cone_calc;

typedef struct s_cone_quad
{
	double	da;
	double	va;
	double	d2;
	double	dv;
	double	v2;
	double	quad_a;
	double	quad_b;
	double	quad_c;
	double	dis;
}	t_cone_quad;

typedef struct s_cone_uv
{
	t_vec3	k;
	t_vec3	ra;
	t_vec3	tan;
	double	x;
	double	y;
}	t_cone_uv;

typedef struct s_bump_ctx
{
	const t_bump	*b;
	t_vec3			n;
	t_vec3			t;
	t_vec3			bta;
}	t_bump_ctx;

typedef struct s_hit_record
{
	t_vec3		p;
	t_vec3		normal;
	double		t;
	int			front_face;
	t_rgb		rgb;
	t_material	*mat;
	double		u;
	double		v;
	t_vec3		tangent;
	t_vec3		bitangent;
	t_bump		*bump;
}				t_hit_record;

typedef struct s_bvh_hit_pair
{
	int				hit_left;
	int				hit_right;
	t_hit_record	left_rec;
	t_hit_record	right_rec;
}	t_bvh_hit_pair;

typedef struct s_ray_color_vars
{
	t_ray			current_ray;
	t_rgb			final_color;
	t_rgb			throughput;
	t_hit_record	rec;
	int				depth;
	t_vec3			bumped;
	int				front;
	t_rgb			direct_light;
	t_ray			scattered;
	t_rgb			attenuation;
	t_rgb			direct_contrib;
	double			max_throughput;
	double			brightness;
	t_rgb			sky;
}				t_ray_color_vars;

typedef struct s_shadow_calc
{
	t_vec3			offset;
	t_vec3			sample_point;
	t_vec3			to_light;
	double			distance;
	t_vec3			light_dir;
	t_ray			shadow_ray;
	t_hit_record	shadow_rec;
	double			diffuse;
	t_vec3			view_dir;
}	t_shadow_calc;

typedef struct s_phong_spec
{
	t_rgb				*light_contrib;
	t_light				*light;
	const t_hit_record	*rec;
	t_shadow_calc		*sc;
}	t_phong_spec;

int				hit_sphere_obj(const t_sphere *s, t_ray *r, t_hit_range range,
					t_hit_record *rec);
int				hit_plane_obj(const t_plane *pl, t_ray *r, t_hit_range range,
					t_hit_record *rec);
int				hit_cylinder_obj(const t_cylinder *cyl, t_ray *r,
					t_hit_range range, t_hit_record *rec);
int				hit_pyramid_obj(const t_pyramid *py, t_ray *r,
					t_hit_range range, t_hit_record *rec);
int				hit_cone_obj(const t_cone *co, t_ray *r, t_hit_range range,
					t_hit_record *rec);
int				hit_triangle_obj(const t_triangle *tri, t_ray *r,
					t_hit_range range, t_hit_record *rec);
int				hit_triangle_test(const t_triangle *tri, t_ray *r,
					t_hit_record *rec, t_tri_hit_ctx ctx);
void			set_face_normal(t_hit_record *rec, const t_ray *r,
					const t_vec3 *outw);
int				hittable_hit(const t_obj *o, t_ray *r, t_hit_range range,
					t_hit_record *rec);
int				world_hit(const t_obj_list *list, t_ray *r, t_hit_range range,
					t_hit_record *out);
int				world_hit_bvh(t_bvh_ctx *ctx, t_hit_record *out);
t_vec3			bump_perturb_from_uv(t_bump_ctx *ctx, double u, double v);

int				aabb_hit(const t_aabb *box, const t_ray *r, double tmin,
					double tmax);
t_aabb			aabb_expand_point(t_aabb box, t_vec3 point);
t_aabb			aabb_union(t_aabb a, t_aabb b);
t_vec3			aabb_centroid(const t_aabb *box);
t_aabb			get_sphere_bounds(const t_sphere *s);
t_aabb			get_plane_bounds(const t_plane *p);
t_aabb			get_cylinder_bounds(const t_cylinder *c);
t_aabb			get_triangle_bounds(const t_triangle *t);
t_aabb			get_cone_bounds(const t_cone *co);
t_aabb			get_object_bounds(const t_obj *obj);
t_bvh_node		*build_bvh(t_obj_list *list);
int				bvh_hit(t_bvh_node *node, t_ray *r, t_hit_range range,
					t_hit_record *rec);
void			free_bvh(t_bvh_node *node);
int				get_longest_axis(t_aabb *box);
double			get_centroid_component(t_obj *obj, int axis);
void			swap_objects(t_obj **a, t_obj **b);
int				partition_objects(t_obj **objects, int count, int axis);
t_aabb			compute_bounds(t_obj **objects, int count);
int				cyl_valid_axis(const t_cylinder *cyl, t_cyl_hit *ch);
int				cyl_select_root(t_cyl_hit *ch, t_ray *r, t_hit_range range);
int				cyl_cap_hit_check(t_cyl_cap_ctx *ctx, t_ray *r, t_cyl_hit *ch);
void			check_best_hit(t_cyl_hit *ch, t_hit_range range,
					t_cyl_best *best);
void			compute_cyl_uv(const t_cylinder *cyl, t_cyl_hit *ch,
					t_vec3 best_p, t_hit_record *rec);
t_vec3			norm_or_zero(t_vec3 v);
void			build_basis(t_vec3 up, t_vec3 *right, t_vec3 *fwd);
int				check_uv_bounds(t_py_tri_uv *uv);
void			compute_base_corners(t_py_basis *basis, t_py_verts *v);
void			compute_uv(const t_pyramid *py, t_hit_record *rec);
void			set_tangent_basis(t_hit_record *rec);
int				cone_base_hit(const t_cone *co, t_ray *r, t_hit_range range,
					t_hit_record *rec);
void			compute_cone_uv(const t_cone *co, t_hit_record *rec,
					t_cone_uv *uv);
void			set_cone_tangent_basis(const t_cone *co, t_hit_record *rec);
int				validate_cone_hit(const t_cone *co, t_ray *r, t_cone_calc *cc,
					double t);
void			set_cone_normal(t_ray *r, t_cone_calc *cc, double t,
					t_hit_record *rec);
void			init_cone_calc(const t_cone *co, t_ray *r, t_cone_calc *cc);
int				solve_cone_quad(t_ray *r, t_cone_calc *cc, t_cone_quad *cq);
int				select_cone_root(t_cone_quad *cq, t_hit_range range,
					double *t);
int				check_base_distance(t_vec3 p, t_vec3 center, double radius);

#endif