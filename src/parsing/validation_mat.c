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
		*outp = &o->data.cylinder.mat;
		return (0);
	}
	if (o->type == CONE)
	{
		*base_len = 6;
		*albedo = o->data.cone.rgb;
		*outp = &o->data.cone.mat;
		return (0);
	}
	if (o->type == TRIANGLE)
	{
		*base_len = 5;
		*albedo = o->data.triangle.rgb;
		*outp = &o->data.triangle.mat;
		return (0);
	}
	return (1);
}

static int	parse_ri_token(const char *mstr, double *ri)
{
	const char	*s;

	*ri = 1.5;
	s = ft_strchr(mstr, ':');
	if (!s)
		return (0);
	s++;
	if (parse_double(s, ri))
		return (1);
	if (*ri <= 0.0)
		*ri = 1.5;
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
	int			i;

	if (mat_base_for_obj(o, &base_len, &albedo, &out))
		return (1);
	has_mat = 0;
	mat_idx = base_len;
	if (len > base_len && (tokens[mat_idx][0] == 'L'
			|| tokens[mat_idx][0] == 'M' || tokens[mat_idx][0] == 'G'))
		has_mat = 1;
	mstr = has_mat ? tokens[mat_idx] : "L";
	if (create_material_from_token(mstr, albedo, out))
		return (1);
	i = mat_idx + (has_mat ? 1 : 0);
	while (i < len)
	{
		if (ft_strncmp(tokens[i], "tx:", 3) == 0)
		{
			const char *rest = tokens[i] + 3;
			double scale = 1.0;
			if (ft_strncmp(rest, "checker", 7) != 0)
				return (1);
			rest += 7;
			if (*rest == ':')
			{
				rest++;
				if (parse_double(rest, &scale))
					return (1);
				if (scale <= 0.0)
					scale = 1.0;
			}
			(*out)->texture_type = CHECKER;
			(*out)->texture_scale = scale;
			(*out)->texture_a = (t_rgb){255.0, 255.0, 255.0};
			(*out)->texture_b = (t_rgb){0.0, 0.0, 0.0};
		}
		i++;
	}
	return (0);
}
