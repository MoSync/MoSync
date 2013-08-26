/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
