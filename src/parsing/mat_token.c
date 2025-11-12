/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:06:47 by nweber            #+#    #+#             */
/*   Updated: 2025/11/12 12:27:45 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Selects and creates a material based on the first char of mstr.
 * L=Lambertian, M=Metal (roughness 0), P=Phong, G=Glass.
 * @param mstr token (e.g. "P:64")
 * @param albedo base color
 * @param out receives allocated material
 * @return 0 success, 1 failure/unknown
 */
int	create_material_from_token(const char *mstr, t_rgb albedo,
			t_material **out)
{
	if (mstr[0] == 'L')
		return (*out = material_lambertian(albedo), *out == NULL);
	if (mstr[0] == 'M')
		return (*out = material_metal(albedo, 0.0), *out == NULL);
	if (mstr[0] == 'P')
		return (create_phong_material(mstr, albedo, out));
	if (mstr[0] == 'G')
		return (create_glass_material(mstr, out));
	return (1);
}

/**
 * Sets base_len, albedo pointer, and material pointer for sphere/plane.
 * @return 0 if handled, 1 if type mismatch.
 */
static int	mat_base_sp_pl(t_obj *o, int *base_len,
			t_rgb *albedo, t_material ***outp)
{
	if (o->type != SPHERE && o->type != PLANE)
		return (1);
	*base_len = 4;
	if (o->type == SPHERE)
	{
		*albedo = o->data.sphere.rgb;
		*outp = &o->data.sphere.mat;
	}
	else
	{
		*albedo = o->data.plane.rgb;
		*outp = &o->data.plane.mat;
	}
	return (0);
}

/**
 * Sets base info for cylinder / pyramid / cone.
 * @return 0 if handled, 1 if type mismatch.
 */
static int	mat_base_cycopc(t_obj *o, int *base_len,
			t_rgb *albedo, t_material ***outp)
{
	if (o->type != CYLINDER && o->type != PYRAMID && o->type != CONE)
		return (1);
	*base_len = 6;
	if (o->type == CYLINDER)
	{
		*albedo = o->data.cylinder.rgb;
		*outp = &o->data.cylinder.mat;
	}
	else if (o->type == PYRAMID)
	{
		*albedo = o->data.pyramid.rgb;
		*outp = &o->data.pyramid.mat;
	}
	else
	{
		*albedo = o->data.cone.rgb;
		*outp = &o->data.cone.mat;
	}
	return (0);
}

/**
 * Sets base info for triangle.
 * @return 0 if handled, 1 if type mismatch.
 */
static int	mat_base_triangle(t_obj *o, int *base_len,
			t_rgb *albedo, t_material ***outp)
{
	if (o->type != TRIANGLE)
		return (1);
	*base_len = 5;
	*albedo = o->data.triangle.rgb;
	*outp = &o->data.triangle.mat;
	return (0);
}

/**
 * Dispatch helper: finds matching object type and fills base info.
 * @return 0 success, 1 no match / error.
 */
int	mat_base_for_obj(t_obj *o, int *base_len,
			t_rgb *albedo, t_material ***outp)
{
	if (!o)
		return (1);
	if (!mat_base_sp_pl(o, base_len, albedo, outp))
		return (0);
	if (!mat_base_cycopc(o, base_len, albedo, outp))
		return (0);
	if (!mat_base_triangle(o, base_len, albedo, outp))
		return (0);
	return (1);
}
