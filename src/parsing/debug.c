#include "mini_rt.h"

void	print_vec3(const char *name, t_vec3 v)
{
	printf("%s(%.6f, %.6f, %.6f)", name, v.x, v.y, v.z);
}

void	print_rgb(const char *name, t_rgb c)
{
	printf("%s(%d, %d, %d)", name, (int)c.r, (int)c.g, (int)c.b);
}

static void	print_ambient(const t_ambient *a)
{
	if (!a)
		return ;
	printf("Ambient:\n");
	printf("  ratio: %.6f\n", a->lighting);
	printf("  ");
	print_rgb("color=", a->color);
	printf("\n");
}

static void	print_camera(const t_camera *c)
{
	if (!c)
		return ;
	printf("Camera:\n");
	printf("  ");
	print_vec3("pos=", c->cords);
	printf("\n  ");
	print_vec3("dir=", c->orientation);
	printf("\n");
	printf("  fov: %.6f\n", c->foc);
}

static void	print_lights(const t_light_list *lst)
{
	const t_light	*cur;
	size_t			i;

	printf("Lights (count=%zu):\n", lst ? lst->size : 0);
	if (!lst)
		return ;
	cur = lst->head;
	i = 0;
	while (cur)
	{
		printf("  [%zu] ", i);
		print_vec3("pos=", cur->cords);
		printf(" ");
		print_rgb("color=", cur->color);
		printf("\n");
		cur = cur->next;
		i++;
	}
}

static void print_material(t_material *mat)
{
	if (!mat)
	{
		printf(" mat=NULL ");
		return;
	}
	if (mat->type == LAMBERTIAN)
		printf(" mat=LAMBERTIAN ");
	if (mat->type == METAL)
		printf(" mat=METAL ");
}

static void	print_object_one(const t_obj *o, size_t idx)
{
	if (o->type == SPHERE)
	{
		printf("  [%zu] sp: ", idx);
		print_vec3("center=", o->data.sphere.cords);
		printf(" dia=%.6f ", o->data.sphere.diameter);
		print_rgb("rgb=", o->data.sphere.rgb);
		print_material(o->data.sphere.mat);
		printf("\n");
	}
	else if (o->type == PLANE)
	{
		printf("  [%zu] pl: ", idx);
		print_vec3("point=", o->data.plane.cords);
		printf(" ");
		print_vec3("norm=", o->data.plane.norm);
		printf(" ");
		print_rgb("rgb=", o->data.plane.rgb);
		printf("\n");
	}
	else if (o->type == CYLINDER)
	{
		printf("  [%zu] cy: ", idx);
		print_vec3("center=", o->data.cylinder.cords);
		printf(" ");
		print_vec3("axis=", o->data.cylinder.norm);
		printf(" dia=%.6f h=%.6f ", o->data.cylinder.diameter,
			o->data.cylinder.height);
		print_rgb("rgb=", o->data.cylinder.rgb);
		printf("\n");
	}
}

static void	print_objects(const t_obj_list *lst)
{
	const t_obj	*cur;
	size_t		i;

	printf("Objects (count=%zu):\n", lst ? lst->size : 0);
	if (!lst)
		return ;
	cur = lst->head;
	i = 0;
	while (cur)
	{
		print_object_one(cur, i);
		cur = cur->next;
		i++;
	}
}

void	print_scene(const t_data *scene)
{
	if (!scene)
		return ;
	printf("==== Parsed Scene ====\n");
	print_ambient(&scene->ambiente);
	print_camera(&scene->camera);
	print_lights(scene->light_list);
	print_objects(scene->objects);
	printf("======================\n");
}
