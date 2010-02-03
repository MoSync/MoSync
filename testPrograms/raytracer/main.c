/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <ma.h>
#include "vector.h"
#include "raytracer.h"

#define EPSILON 0.000000001

Primitive *g_primitives[128];
int g_num_primitives = 0;

Primitive *g_lights[128];
int g_num_lights = 0;

byte *g_framebuffer = NULL;
MAFrameBufferInfo g_framebuffer_info;

Camera g_camera;

RaytraceFptr raytrace_fptrs[16];
CalculateFptr calculate_fptrs[16];

int g_render_width, g_render_height;

int color_to_native(Color *c) {
	return
	(((int)c->r>>(8-g_framebuffer_info.redBits))<<g_framebuffer_info.redShift) |
	(((int)c->g>>(8-g_framebuffer_info.greenBits))<<g_framebuffer_info.greenShift) |
	(((int)c->b>>(8-g_framebuffer_info.blueBits))<<g_framebuffer_info.blueShift);
}

Primitive* create_sphere(float x, float y, float z, float r, int cr, int cg, int cb) {
	Primitive* p = (Primitive*)malloc(sizeof(Primitive));
	Sphere *s = (Sphere*)malloc(sizeof(Sphere));
	s->radius = r;
	s->radius_2 = r*r;
	s->radius_recip = 1.0f/r;
	p->data = s;
	p->pos.x = x;
	p->pos.y = y;
	p->pos.z = z;
	p->color.r = cr;
	p->color.g = cg;
	p->color.b = cb;
	p->type = PTYPE_SPHERE;
	return p;
};

Primitive* create_light(float x, float y, float z, int cr, int cg, int cb) {
	Primitive* p = (Primitive*)malloc(sizeof(Primitive));
	p->data = 0;
	p->pos.x = x;
	p->pos.y = y;
	p->pos.z = z;
	p->color.r = cr;
	p->color.g = cg;
	p->color.b = cb;
	p->type = PTYPE_LIGHT;
	return p;
};

Primitive* create_plane(float nx, float ny, float nz, float offset, int cr, int cg, int cb) {
	Primitive* p = (Primitive*)malloc(sizeof(Primitive));
	Plane *pl = (Plane*)malloc(sizeof(Plane));
	set_vec3(&pl->normal, nx, ny, nz);
	pl->offset = offset;
	p->data = pl;
	p->pos.x = nx*offset;
	p->pos.y = ny*offset;
	p->pos.z = nz*offset;
	p->color.r = cr;
	p->color.g = cg;
	p->color.b = cb;
	p->type = PTYPE_PLANE;
	return p;
};

BOOL raytrace_sphere(Primitive *p, Ray *ray) {
	Sphere *s = (Sphere*) p->data;
	vec3 v = ray->origin;
	sub_vec3(&v, &p->pos);
	float b = dot_vec3(&v, &ray->direction) * 2.0f;
	float c = dot_vec3(&v,&v) - s->radius_2;  // per frame constant (precalc later)
	float d = b*b - 4*c;
	if(d<0) return FALSE; // no intersection, if == 0 one intersection, else two
	d = sqrt(d);
	float t = (-b+d); // ie /2 , do it later
	float t2 = (-b-d); // ie /2, do it later
	if(t<t2) {
		if(t<0) t = t2;
	} else {
		if(t2>0) t = t2;
	}
	if(t<0) return FALSE;
	t*=0.5f;
	ray->t = t;
	return TRUE;
}

BOOL raytrace_plane(Primitive *p, Ray *ray) {
	Plane *plane = (Plane*)p->data;
	float t;
	float denom = dot_vec3(&plane->normal, &ray->direction);

	if(fabs(denom) < EPSILON)
	{
		return FALSE;
	}

	t = (plane->offset - dot_vec3(&plane->normal, &ray->origin)) / denom;

	if(t<EPSILON) return FALSE;

	ray->t = t;

	return TRUE;
}

void  calculate_sphere_data(Primitive *p, Ray *ray) {
	Sphere *s = (Sphere*) p->data;

	// calculate normal
	ray->normal = ray->intersection;
	sub_vec3(&ray->normal,&p->pos);
	mul_vec3(&ray->normal, s->radius_recip);
}

void calculate_plane_data(Primitive *p, Ray *ray) {
	ray->normal = ((Plane*)p->data)->normal;
}

void calculate_color(Primitive *p, Ray *ray) {
	Color c = {0,0,0,0};
	float intensity = 0;
	int i,j;
	Ray shadow_ray;
	shadow_ray.origin = ray->intersection;

	for(i = 0; i < g_num_lights; i++) {
		shadow_ray.direction = g_lights[i]->pos;
		sub_vec3(&shadow_ray.direction, &shadow_ray.origin);
	/*
		vec3 offset = shadow_ray.direction;
		mul_vec3(&offset, 0.000001);
		add_vec3(&shadow_ray.origin, &offset);
		*/

		norm_vec3(&shadow_ray.direction);

		BOOL occluded_light = FALSE;
		for(j = 0; j < g_num_primitives; j++) {
			if(p == g_primitives[j]) continue;
			if(raytrace_fptrs[g_primitives[j]->type]
			               (g_primitives[j], &shadow_ray)) {
				if(shadow_ray.t>0 && shadow_ray.t<1) {
					occluded_light = TRUE;
					break;
				}
			}
		}
		//sub_vec3(&shadow_ray.origin, &offset);

		if(occluded_light == FALSE) {
			vec3 light_dir = g_lights[i]->pos;
			sub_vec3(&light_dir, &ray->intersection);
			norm_vec3(&light_dir);
			float d = dot_vec3(&light_dir, &ray->normal);
			if(d>0) {
				c.r+=((g_lights[i]->color.r*p->color.r)>>8)*d;
				c.g+=((g_lights[i]->color.g*p->color.g)>>8)*d;
				c.b+=((g_lights[i]->color.b*p->color.b)>>8)*d;
			}
		}
	}

	if(c.r > 0xff) c.r = 0xff;
	if(c.g > 0xff) c.g = 0xff;
	if(c.b > 0xff) c.b = 0xff;
	if(c.r < 0) c.r = 0;
	if(c.g < 0) c.g = 0;
	if(c.b < 0) c.b = 0;

	ray->color = color_to_native(&c);
}

BOOL raytrace_light(Primitive *p, Ray *ray) {
	return FALSE;
}

BOOL raytrace(Ray *ray) {
	int i;
	float min_dist = 100000.0;
	Ray nearest_ray;
	int num_primitives = 0;
	int nearest_primitive = 0;
	for(i = 0; i < g_num_primitives; i++) {
		if(raytrace_fptrs[g_primitives[i]->type]
		               (g_primitives[i], ray)) {
			if(ray->t<min_dist) {
				min_dist = ray->t;
				nearest_ray = *ray;
				nearest_primitive = i;
			}
			num_primitives++;
		}
	}
	if(num_primitives>0) {
		*ray = nearest_ray;

		ray->intersection = ray->direction;
		mul_vec3(&ray->intersection, ray->t);
		add_vec3(&ray->intersection, &ray->origin);

		calculate_fptrs[g_primitives[nearest_primitive]->type](g_primitives[nearest_primitive], ray);
		calculate_color(g_primitives[nearest_primitive], ray);

		return TRUE;
	} else {
		return FALSE;
	}
}

void init_raytracer() {
	raytrace_fptrs[PTYPE_SPHERE] = raytrace_sphere;
	calculate_fptrs[PTYPE_SPHERE] = calculate_sphere_data;

	raytrace_fptrs[PTYPE_PLANE] = raytrace_plane;
	calculate_fptrs[PTYPE_PLANE] = calculate_plane_data;

	raytrace_fptrs[PTYPE_LIGHT] = raytrace_light;

}

void add_primitive(Primitive *p) {
	g_primitives[g_num_primitives++] = p;
}

void add_light(Primitive *l) {
	g_lights[g_num_lights++] = l;
	add_primitive(l);
}

void set_camera(Camera *cam, vec3 pos, vec3 target, vec3 up)
{
	cam->fov = PI*0.5f;
	cam->direction = target;
	sub_vec3(&cam->direction, &pos);
	norm_vec3(&cam->direction);
	cam->pos = pos;
	cam->target = target;
	cam->up = up;

	vec3 z_vec = cam->direction;
	vec3 x_vec = cross_vec3_new(&up,&z_vec);
	//invsign_vec3(&z_vec);
	vec3 y_vec = cross_vec3_new(&z_vec,&x_vec);
	//invsign_vec3(&z_vec);

	float aspect_ratio;
	aspect_ratio = (float)g_render_height/(float)g_render_width;

	set_vec3(&cam->viewplane[0], -1.0f,  aspect_ratio, cos(cam->fov/2));
	set_vec3(&cam->viewplane[1],  1.0f,  aspect_ratio, cos(cam->fov/2));
	set_vec3(&cam->viewplane[2],  1.0f, -aspect_ratio, cos(cam->fov/2));
	set_vec3(&cam->viewplane[3], -1.0f, -aspect_ratio, cos(cam->fov/2));

	ident_mtx44(cam->mtx);

	cam->mtx[0] = x_vec.x;
	cam->mtx[1] = x_vec.y;
	cam->mtx[2] = x_vec.z;

	cam->mtx[4] = y_vec.x;
	cam->mtx[5] = y_vec.y;
	cam->mtx[6] = y_vec.z;

	cam->mtx[8] = z_vec.x;
	cam->mtx[9] = z_vec.y;
	cam->mtx[10] = z_vec.z;

	// invert
	invert_mtx44(cam->mtx);

	cam->mtx[3] = pos.x;
	cam->mtx[7] = pos.y;
	cam->mtx[11] = pos.z;

	//print_mtx44(cam->mtx);

	cam->viewplane[0] = mul_mtx44_vec3(cam->mtx, &cam->viewplane[0]);
	cam->viewplane[1] = mul_mtx44_vec3(cam->mtx, &cam->viewplane[1]);
	cam->viewplane[2] = mul_mtx44_vec3(cam->mtx, &cam->viewplane[2]);
	cam->viewplane[3] = mul_mtx44_vec3(cam->mtx, &cam->viewplane[3]);
}

void init_graphics() {
	if(!g_framebuffer) {
		int res = maFrameBufferGetInfo(&g_framebuffer_info);
		if(res < 0)
			maPanic(res, "Framebuffer info failure!");
		g_framebuffer = malloc(g_framebuffer_info.sizeInBytes);
		res = maFrameBufferInit(g_framebuffer);
		if(res < 0)
			maPanic(res, "Framebuffer init failure!");
	}

	g_render_width = (g_framebuffer_info.width);
	g_render_height = (g_framebuffer_info.height);
}

void update_events() {
	MAEvent e;
	while(maGetEvent(&e)) {
		switch(e.type) {
		case EVENT_TYPE_CLOSE:
		case EVENT_TYPE_KEY_PRESSED:
			maExit(0);
		}
	}
	maUpdateScreen();
	maResetBacklight();
}

void render_scene() {
	Ray ray;
	int i, j;

	ray.origin = g_camera.pos;

	vec3 left = g_camera.viewplane[0];
	vec3 right = g_camera.viewplane[1];

	vec3 left_slope = g_camera.viewplane[3];
	sub_vec3(&left_slope, &left);
	mul_vec3(&left_slope, 1.0f/(float)g_render_height);

	vec3 right_slope = g_camera.viewplane[2];
	sub_vec3(&right_slope, &right);
	mul_vec3(&right_slope, 1.0f/(float)g_render_height);

	vec3 slope;
	vec3 lookat;

	int index = 0;
	int pitch = g_framebuffer_info.pitch>>(g_framebuffer_info.bytesPerPixel>>1);

	for(j = 0; j < g_render_height; j++) {
		slope = right;
		sub_vec3(&slope, &left);
		mul_vec3(&slope, 1.0/(float)g_render_width);
		lookat = left;
		for(i = 0; i < g_render_width; i++) {
			ray.direction = lookat;
			sub_vec3(&ray.direction, &ray.origin);
			norm_vec3(&ray.direction);
			if(raytrace(&ray)) {
				switch(g_framebuffer_info.bytesPerPixel) {
				case 2: ((short*)g_framebuffer)[index] = (short) (ray.color&0xffff); break;
				case 4: ((int*)g_framebuffer)[index] = (int) ray.color; break;
				}
			}

			index++;
			add_vec3(&lookat, &slope);
		}
		index+=-g_render_width+pitch;
		add_vec3(&left, &left_slope);
		add_vec3(&right, &right_slope);
		update_events();
	}
}

int MAMain() {
	int i, j;
	init_graphics();
	init_raytracer();

	set_camera(&g_camera, create_vec3(0.0f, 2.0f, -2.0f), create_vec3(0, 0, 0), create_vec3(0, 1, 0));

	for(i = -1; i <= 1; i++)
	for(j = -1; j <= 1; j++)
		add_primitive(create_sphere(i, 0, j, 0.75f, 0xff, 0x00, 0x00));

	add_light(create_light(-2.0, 2.0, -2.0f, 0xff, 0xff, 0xff));
	add_primitive(create_plane(0, 1, 0, 0, 0x00, 0x00, 0xff));
	while(1) {
		update_events();
		//maSetColor(0x0000ff),
		//maFillRect(0, 0, 240, 320);
		double time = (double) maGetMilliSecondCount()*0.001;
		set_camera(&g_camera, create_vec3(cos(time)*2, 2.0f, sin(time)*2), create_vec3(0, 0, 0), create_vec3(0, 1, 0));
		render_scene();
		//update_events();
		//maUpdateScreen();
	}

    return 0;
}
