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

#include "Vector3.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

#include <maheap.h>

/*void *malloc(int s) {
	return NULL;
} */

Vector3* Vec3_alloc(double x, double y, double z) {
	Vector3* ret = (Vector3*) malloc(sizeof(Vector3));
	ret->x = x;
	ret->y = y;
	ret->z = z;
	return ret;
}

Vector3 Vec3_create(double x, double y, double z) {
	Vector3 ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

Vector3 Vec3_add(const Vector3* v1, const Vector3* v2) {
	Vector3 ret;
	ret.x = v1->x + v2->x;
	ret.y = v1->y + v2->y;
	ret.z = v1->z + v2->z;
	return ret;
}

Vector3 Vec3_sub(const Vector3* v1, const Vector3* v2) {
	Vector3 ret;
	ret.x = v1->x - v2->x;
	ret.y = v1->y - v2->y;
	ret.z = v1->z - v2->z;
	return ret;
}

#define SQR(x) ((x)*(x))
#define V3_MAGNITUDE(v) sqrt(SQR(v->x)+SQR(v->y)+SQR(v->z))


double Vec3_magnitude(const Vector3* v) {
	return V3_MAGNITUDE(v);
}

Vector3* Vec3_normalize(Vector3* v) {
	double l = V3_MAGNITUDE(v);
	v->x /= l;
	v->y /= l;
	v->z /= l;
	return v;
}

Vector3 Vec3_toUnit(const Vector3* v) {
	double l = V3_MAGNITUDE(v);
	Vector3 ret = *v;
	ret.x /= l;
	ret.y /= l;
	ret.z /= l;
	return ret;
}

Vector3 Vec3_mul(const Vector3* v, double d) {
	Vector3 ret = *v;
	ret.x *= d;
	ret.y *= d;
	ret.z *= d;
	return ret;
}

double Vec3_dot(const Vector3* v1, const Vector3* v2) {
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

double Vec3_angle(const Vector3* v1, const Vector3* v2) {
	/*Vector3 u1 = Vec3_toUnit(v1);
	Vector3 u2 = Vec3_toUnit(v2);
	return acos(Vec3_dot(&u1, &u2));*/
	return 0;
}

Vector3 Vec3_cross(const Vector3* v1, const Vector3* v2) {
	Vector3 returnValue;
	returnValue.x = v1->y * v2->z - v1->z * v2->y;
	returnValue.y = v1->z * v2->x - v1->x * v2->z;
	returnValue.z = v1->x * v2->y - v1->y * v2->x;
	return returnValue;
}

static char Vec3_toStringBuf[256];

char* Vec3_toString(const Vector3* v) {
	//sprintf((char*)Vec3_toStringBuf, "{%f,%f,%f}", v->x, v->y, v->z);
	//return toStringBuf;
	Vec3_toStringBuf[0] = 'a';
	Vec3_toStringBuf[1] = '\0';
	return Vec3_toStringBuf;
}
