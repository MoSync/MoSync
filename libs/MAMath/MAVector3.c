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

#include "MAVector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Vector3* Vec3_alloc(float x, float y, float z) {
	Vector3* ret = (Vector3*) malloc(sizeof(Vector3));
	ret->x = x;
	ret->y = y;
	ret->z = z;
	return ret;
}

Vector3 Vec3_create(float x, float y, float z) {
	Vector3 ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

Vector3 Vec3_add(Vector3* v1, Vector3* v2) {
	Vector3 ret;
	ret.x = v1->x + v2->x;
	ret.y = v1->y + v2->y;
	ret.z = v1->z + v2->z;
	return ret;
}

Vector3 Vec3_sub(Vector3* v1, Vector3* v2) {
	Vector3 ret;
	ret.x = v1->x - v2->x;
	ret.y = v1->y - v2->y;
	ret.z = v1->z - v2->z;
	return ret;
}

#define SQR(x) ((x)*(x))
#define V3_MAGNITUDE(v) sqrt(SQR(v->x)+SQR(v->y)+SQR(v->z))


float Vec3_magnitude(Vector3* v) {
	return V3_MAGNITUDE(v);
}

Vector3* Vec3_normalize(Vector3* v) {
	float l = V3_MAGNITUDE(v);
	v->x /= l;
	v->y /= l;
	v->z /= l;
	return v;
}

Vector3 Vec3_toUnit(Vector3* v) {
	float l = V3_MAGNITUDE(v);
	Vector3 ret = *v;
	ret.x /= l;
	ret.y /= l;
	ret.z /= l;
	return ret;
}

float Vec3_dot(Vector3* v1, Vector3* v2) {
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float Vec3_angle(Vector3* v1, Vector3* v2) {
	Vector3 u1 = Vec3_toUnit(v1);
	Vector3 u2 = Vec3_toUnit(v2);
	return acos(Vec3_dot(&u1, &u2));
}

Vector3 Vec3_cross(Vector3* v1, Vector3* v2) {
	Vector3 returnValue;
	returnValue.x = v1->y * v2->z - v1->z * v2->y;
	returnValue.y = v1->z * v2->x - v1->x * v2->z;
	returnValue.z = v1->x * v2->y - v1->y * v2->x;
	return returnValue;
}

static char toStringBuf[256];

char* Vec3_toString(Vector3* v) {
	sprintf((char*)toStringBuf, "{%f,%f,%f}", v->x, v->y, v->z);
	return toStringBuf;
}
