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

#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#define PTYPE_SPHERE 0
#define PTYPE_LIGHT 1
#define PTYPE_PLANE 2

#define PI 3.14159

typedef struct Color_t {
	int r, g, b, a;
} Color;

typedef struct Primitive_t {
	int type;
	vec3 pos;
	Color color;
	void *data;
} Primitive;

typedef struct Sphere_t {
	float radius;
	float radius_2;
	float radius_recip;
} Sphere;

typedef struct Plane_t {
	vec3 normal;
	float offset;
} Plane;

typedef struct Camera_t
{
	vec3	pos,
			up,
			target,
			direction;

	vec3	viewplane[4];
	float	mtx[16];
	float	fov;
} Camera;

typedef struct Ray_t {
	// in
	vec3 origin, direction;

	// out
	vec3 	intersection,
			normal;
	float t;
	int color;
} Ray;

typedef BOOL (*RaytraceFptr)(Primitive*, Ray*);
typedef void (*CalculateFptr)(Primitive*, Ray*);


#endif /* _RAYTRACER_H_ */
