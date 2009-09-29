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
