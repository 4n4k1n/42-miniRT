#include "mini_rt.h"

static int	mat_base_for_obj(t_obj *o, int *base_len, t_rgb *albedo,
			t_material ***outp)
{
	if (!o)
		return (1);
	if (o->type == SPHERE)
	{
		*base_len = 4;
		*albedo = o->data.sphere.rgb;
		*outp = &o->data.sphere.mat;
		return (0);
	}
	if (o->type == PLANE)
	{
		*base_len = 4;
		*albedo = o->data.plane.rgb;
		*outp = &o->data.plane.mat;
		return (0);
	}
	if (o->type == CYLINDER)
	{
		*base_len = 6;
		*albedo = o->data.cylinder.rgb;
		*outp = &o->data.cylinder.mat;
		return (0);
	}
	if (o->type == PYRAMID)
	{
		*base_len = 6;
		*albedo = o->data.cylinder.rgb;
		*outp = &o->data.pyramid.mat;
		return (0);
	}
	if (o->type == CONE)
	{
		*base_len = 6;
		*albedo = o->data.cone.rgb;
		*outp = &o->data.cone.mat;
		return (0);
	}
	return (1);
}

static int	parse_ri_token(const char *mstr, double *ri)
{
	const char	*num;
	double		val;

	num = NULL;
	if (mstr[1] == ':')
		num = mstr + 2;
	else if (mstr[1] != '\0')
		num = mstr + 1;
	if (num && *num != '\0')
		val = ft_atof(num);
	else
		val = 1.5;
	if (val <= 0.0)
		val = 1.5;
	*ri = val;
	return (0);
}

static int	create_material_from_token(const char *mstr, t_rgb albedo,
			t_material **out)
{
	double	ri;

	if (mstr[0] == 'L')
	{
		*out = material_lambertian(albedo);
		return (*out == NULL);
	}
	if (mstr[0] == 'M')
	{
		*out = material_metal(albedo, 0.0);
		return (*out == NULL);
	}
	if (mstr[0] == 'G')
	{
		if (parse_ri_token(mstr, &ri))
			return (1);
		*out = material_dielectric(ri);
		return (*out == NULL);
	}
	return (1);
}

int	parse_material(char **tokens, int len, t_obj *o)
{
	int			base_len;
	int			has_mat;
	int			mat_idx;
	const char	*mstr;
	t_rgb		albedo;
	t_material	**out;

	if (mat_base_for_obj(o, &base_len, &albedo, &out))
		return (1);
	if (len != base_len && len != base_len + 1)
		return (1);
	has_mat = (len == base_len + 1);
	if (!has_mat)
	{
		*out = NULL;
		return (0);
	}
	mat_idx = base_len;
	mstr = tokens[mat_idx];
	if (!mstr || !mstr[0])
		return (1);
	return (create_material_from_token(mstr, albedo, out));
}
