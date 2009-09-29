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

#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct vec3_t
{
	float x,y,z;
} vec3;

typedef float mtx33[9];
typedef float mtx44[16];

inline void set_vec3(vec3 *vec, float x, float y, float z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
}

vec3 create_vec3(float x, float y, float z)
{
	vec3 v;
	set_vec3(&v, x, y, z);
	return v;
}

inline void add_vec3(vec3 *a, const vec3 *b)
{
	a->x+=b->x;
	a->y+=b->y;
	a->z+=b->z;
}

inline void sub_vec3(vec3 *a, const vec3 *b)
{
	a->x-=b->x;
	a->y-=b->y;
	a->z-=b->z;
}

inline void invsign_vec3(vec3 *a)
{
	a->x=-a->x;
	a->y=-a->y;
	a->z=-a->z;
}

inline void mul_vec3(vec3 *a, const float b)
{
	a->x*=b;
	a->y*=b;
	a->z*=b;
}

inline float dot_vec3(const vec3 *a, const vec3 *b)
{
	return a->x*b->x + a->y*b->y + a->z*b->z;
}

inline float length_vec3(const vec3 *vec)
{
	return dot_vec3(vec, vec);
}

inline void norm_vec3(vec3 *vec)
{
	float length = 1.0f/length_vec3(vec) ;
	vec->x*=length;
	vec->y*=length;
	vec->z*=length;
}

// cross
//	i  j  k
//  ax ay az = i*(ay*bz-by*az) - j*(ax*bz-bx*az) + k*(ax*by - bx*ay)
//  bx by bz
inline vec3 cross_vec3_new(const vec3 *a, const vec3 *b)
{
	vec3 newvec;
	set_vec3(&newvec, a->y*b->z-a->z*b->y, -a->x*b->z+a->z*b->x, a->x*b->y-b->x*a->y);
	return newvec;
}

inline void cross_vec3(vec3 *dst, const vec3 *a, const vec3 *b)
{
	set_vec3(dst, a->y*b->z-a->z*b->y, -a->x*b->z+a->z*b->x, a->x*b->y-b->x*a->y);
}

inline void clear_mtx33(float *m)
{
	memset(m,0,9*sizeof(float));
}

inline void clear_mtx44(float *m)
{
	memset(m,0,16*sizeof(float));
}

void ident_mtx33(float *m)
{
	m[0] = 1.0f;
	m[4] = 1.0f;
	m[8] = 1.0f;

	m[1] = 0.0f;
	m[2] = 0.0f;

	m[3] = 0.0f;
	m[5] = 0.0f;

	m[6] = 0.0f;
	m[7] = 0.0f;
}

void ident_mtx44(float *m)
{
	m[0] = 1.0f;
	m[4] = 1.0f;
	m[8] = 1.0f;
	m[15] = 1.0f;

	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[5] = 0.0f;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[9] = 0.0f;
	m[10] = 0.0f;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
}

// only for orthagonal matrices
void invert_mtx44(float *m)
{
	float t[16];
	int h, i, v;
	float tx = -m[3], ty = -m[7], tz = -m[11];
	// first transpose the 3x3 part
	for(h = 0; h < 3; h++) for ( v = 0; v < 3; v++ ) t[h + (v << 2)] = m[v + (h << 2)];
	// copy it back
	for(i = 0; i < 11; i++) m[i] = t[i];

	m[3] = tx * m[0] + ty * m[1] + tz * m[2];
	m[7] = tx * m[4] + ty * m[5] + tz * m[6];
	m[11] = tx * m[8] + ty * m[9] + tz * m[10];
}

void create_rotation_mtx33(float *m, float ax, float ay, float az)
{
	ident_mtx33(m);

	float cx,cy,cz,sx,sy,sz;
	cx = (float)cos(ax);
	sx = (float)sin(ax);
	cy = (float)cos(ay);
	sy = (float)sin(ay);
	cz = (float)cos(az);
	sz = (float)sin(az);

	m[0] = cy * cz;
	m[1] = cy * sz;
	m[2] = -sy;

	m[3] = sx * sy * cz - cx * sz;
	m[4] = sx * sy * sz + cx * cz;
	m[5] = sx * cy;

	m[6] = cx * sy * cz + sx * sz;
	m[7] = cx * sy * sz - sx * cz;
	m[8] = cx * cy;
}


void create_rotation_mtx44(float *m, float ax, float ay, float az)
{
	ident_mtx44(m);

	float cx,cy,cz,sx,sy,sz;
	cx = (float)cos(ax);
	sx = (float)sin(ax);
	cy = (float)cos(ay);
	sy = (float)sin(ay);
	cz = (float)cos(az);
	sz = (float)sin(az);

	m[0] = cy * cz;
	m[1] = cy * sz;
	m[2] = -sy;

	m[4] = sx * sy * cz - cx * sz;
	m[5] = sx * sy * sz + cx * cz;
	m[6] = sx * cy;

	m[8] = cx * sy * cz + sx * sz;
	m[9] = cx * sy * sz - sx * cz;
	m[10] = cx * cy;
}

void mul_mtx33(float *a, const float *b)
{
	float res[9];

	res[0] = (a[0])*(b[0]) + (a[1])*(b[3]) + (a[2])*(b[6]);
	res[1] = (a[0])*(b[1]) + (a[1])*(b[4]) + (a[2])*(b[7]);
	res[2] = (a[0])*(b[2]) + (a[1])*(b[5]) + (a[2])*(b[8]);

	res[3] = (a[3])*(b[0]) + (a[4])*(b[3]) + (a[5])*(b[6]);
	res[4] = (a[3])*(b[1]) + (a[4])*(b[4]) + (a[5])*(b[7]);
	res[5] = (a[3])*(b[2]) + (a[4])*(b[5]) + (a[5])*(b[8]);

	res[6] = (a[6])*(b[0]) + (a[7])*(b[3]) + (a[8])*(b[6]);
	res[7] = (a[6])*(b[1]) + (a[7])*(b[4]) + (a[8])*(b[7]);
	res[8] = (a[6])*(b[2]) + (a[7])*(b[5]) + (a[8])*(b[8]);

	memcpy(a, &res, 9*sizeof(float));
}


void mul_mtx44(float *a, const float *b)
{
	float res[16];

	res[0] = (a[0])*(b[0]) + (a[1])*(b[4]) + (a[2])*(b[8]) + (a[3])*(b[12]);
	res[1] = (a[0])*(b[1]) + (a[1])*(b[5]) + (a[2])*(b[9]) + (a[3])*(b[13]);
	res[2] = (a[0])*(b[2]) + (a[1])*(b[6]) + (a[2])*(b[10]) + (a[3])*(b[14]);
	res[3] = (a[0])*(b[3]) + (a[1])*(b[7]) + (a[2])*(b[11]) + (a[3])*(b[15]);

	res[4] = (a[4])*(b[0]) + (a[5])*(b[4]) + (a[6])*(b[8]) + (a[7])*(b[12]);
	res[5] = (a[4])*(b[1]) + (a[5])*(b[5]) + (a[6])*(b[9]) + (a[7])*(b[13]);
	res[6] = (a[4])*(b[2]) + (a[5])*(b[6]) + (a[6])*(b[10]) + (a[7])*(b[14]);
	res[7] = (a[4])*(b[3]) + (a[5])*(b[7]) + (a[6])*(b[11]) + (a[7])*(b[15]);

	res[8] = (a[8])*(b[0]) + (a[9])*(b[3]) + (a[10])*(b[8]) + (a[11])*(b[12]);
	res[9] = (a[8])*(b[1]) + (a[9])*(b[4]) + (a[10])*(b[9]) + (a[11])*(b[13]);
	res[10] = (a[8])*(b[2]) + (a[9])*(b[5]) + (a[10])*(b[10]) + (a[11])*(b[14]);
	res[11] = (a[8])*(b[3]) + (a[9])*(b[6]) + (a[10])*(b[11]) + (a[11])*(b[15]);

	res[12] = (a[12])*(b[0]) + (a[13])*(b[3]) + (a[14])*(b[8]) + (a[15])*(b[12]);
	res[13] = (a[12])*(b[1]) + (a[13])*(b[4]) + (a[14])*(b[9]) + (a[15])*(b[13]);
	res[14] = (a[12])*(b[2]) + (a[13])*(b[5]) + (a[14])*(b[10]) + (a[15])*(b[14]);
	res[15] = (a[12])*(b[3]) + (a[13])*(b[6]) + (a[14])*(b[11]) + (a[15])*(b[15]);

	memcpy(a, &res, 9*sizeof(float));
}

void print_mtx44(float *m)
{
	int i, j;
	float *src = (float*)m;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%.5f ", (*src++));
		}
		printf("\n");
	}
}

vec3 mul_mtx33_vec3(const float *m, const vec3 *v)
{
	vec3 res;

	res.x = (m[0])*v->x + (m[1])*v->y + (m[2])*v->z;
	res.y = (m[3])*v->x + (m[4])*v->y + (m[5])*v->z;
	res.z = (m[6])*v->x + (m[7])*v->y + (m[8])*v->z;

	return res;
}



vec3 mul_vec3_mtx33(const vec3 *v, const float *m)
{
	vec3 res;

	res.x = v->x*(m[0]) + v->y*(m[3]) + v->z*(m[6]);
	res.y = v->x*(m[1]) + v->y*(m[4]) + v->z*(m[7]);
	res.z = v->x*(m[2]) + v->y*(m[5]) + v->z*(m[8]);

	return res;
}

vec3 mul_mtx44_vec3(const float *m, const vec3 *v)
{
	vec3 res;

	res.x = (m[0])*v->x + (m[1])*v->y + (m[2])*v->z + (m[3]);  // w-axis = 1
	res.y = (m[4])*v->x + (m[5])*v->y + (m[6])*v->z + (m[7]);
	res.z = (m[8])*v->x + (m[9])*v->y + (m[10])*v->z + (m[11]);

	return res;
}

vec3 mul_vec3_mtx44(const vec3 *v, const float *m)
{
	vec3 res;

	res.x = v->x*(m[0]) + v->y*(m[4]) + v->z*(m[8]) + (m[12]);
	res.y = v->x*(m[1]) + v->y*(m[5]) + v->z*(m[9]) + (m[13]);
	res.z = v->x*(m[2]) + v->y*(m[6]) + v->z*(m[10]) + (m[14]);

	return res;
}

#endif
