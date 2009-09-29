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

#include <madmath.h>

#include "allegro.h"

static Fix _persp_xscale_f;
static Fix _persp_yscale_f;
static Fix _persp_xoffset_f;
static Fix _persp_yoffset_f;

void set_projection_viewport(int x, int y, int w, int h)
{
	ASSERT(w > 0);
	ASSERT(h > 0);

	_persp_xscale_f = w/2;
	_persp_yscale_f = h/2;
	_persp_xoffset_f = x + w/2;
	_persp_yoffset_f = y + h/2;
}

void persp_project_f(myVertex* v)
{
	Fix z1 = Fix(1) / v->z;
	v->x = ((v->x * z1) * _persp_xscale_f) + _persp_xoffset_f;
	v->y = ((v->y * z1) * _persp_yscale_f) + _persp_yoffset_f;
}

void matrix_mul_f(const MATRIX_f *m1, const MATRIX_f *m2, MATRIX_f *out)
{
	MATRIX_f temp;
	int i, j;
	ASSERT(m1);
	ASSERT(m2);
	ASSERT(out);

	if (m1 == out) {
		temp = *m1;
		m1 = &temp;
	}
	else if (m2 == out) {
		temp = *m2;
		m2 = &temp;
	}

	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			out->v[i][j] = (m1->v[0][j] * m2->v[i][0]) +
				(m1->v[1][j] * m2->v[i][1]) +
				(m1->v[2][j] * m2->v[i][2]);
		}

		out->t[i] = (m1->t[0] * m2->v[i][0]) +
			(m1->t[1] * m2->v[i][1]) +
			(m1->t[2] * m2->v[i][2]) +
			m2->t[i];
	}
}

void get_camera_matrix_f(MATRIX_f *m, Fix x, Fix y, Fix z, Fix xfront, Fix yfront, Fix zfront,
												 Fix xup, Fix yup, Fix zup, double fov, Fix aspect)
{
	MATRIX_f camera, scale;
	Fix xside, yside, zside, width, d;
	ASSERT(m);

	/* make 'in-front' into a unit vector, and negate it */
	normalize_vector_f(&xfront, &yfront, &zfront);
	xfront = -xfront;
	yfront = -yfront;
	zfront = -zfront;

	/* make sure 'up' is at right angles to 'in-front', and normalize */
	d = dot_product_f(xup, yup, zup, xfront, yfront, zfront);
	xup -= d * xfront; 
	yup -= d * yfront; 
	zup -= d * zfront;
	normalize_vector_f(&xup, &yup, &zup);

	/* calculate the 'sideways' vector */
	cross_product_f(xup, yup, zup, xfront, yfront, zfront, &xside, &yside, &zside);

	/* set matrix rotation parameters */
	camera.v[0][0] = xside; 
	camera.v[0][1] = yside;
	camera.v[0][2] = zside;

	camera.v[1][0] = xup; 
	camera.v[1][1] = yup;
	camera.v[1][2] = zup;

	camera.v[2][0] = xfront; 
	camera.v[2][1] = yfront;
	camera.v[2][2] = zfront;

	/* set matrix translation parameters */
	camera.t[0] = -(x*xside  + y*yside  + z*zside);
	camera.t[1] = -(x*xup    + y*yup    + z*zup);
	camera.t[2] = -(x*xfront + y*yfront + z*zfront);

	/* construct a scaling matrix to deal with aspect ratio and FOV */
	width = tan(BIN2RAD(64 - fov/2));
	get_scaling_matrix_f(&scale, width, -aspect*width, -1.0);

	/* combine the camera and scaling matrices */
	matrix_mul_f(&camera, &scale, m);
}

void get_x_rotate_matrix_f(MATRIX_f *m, double r)
{
	double c, s;
	ASSERT(m);

	SINCOS(r, s, c);
	*m = identity_matrix_f;

	m->v[1][1] = c;
	m->v[1][2] = -s;

	m->v[2][1] = s;
	m->v[2][2] = c;
}

void normalize_vector_f(Fix *x, Fix *y, Fix *z)
{
	Fix length = 1.0 / vector_length_f(*x, *y, *z);

	*x *= length;
	*y *= length;
	*z *= length;
}

void cross_product_f(Fix x1, Fix y1, Fix z1, Fix x2, Fix y2, Fix z2,
										 Fix *xout, Fix *yout, Fix *zout)
{
	ASSERT(xout);
	ASSERT(yout);
	ASSERT(zout);

	*xout = (y1 * z2) - (z1 * y2);
	*yout = (z1 * x2) - (x1 * z2);
	*zout = (x1 * y2) - (y1 * x2);
}

void get_scaling_matrix_f(MATRIX_f *m, Fix x, Fix y, Fix z)
{
	ASSERT(m);
	*m = identity_matrix_f;

	m->v[0][0] = x;
	m->v[1][1] = y;
	m->v[2][2] = z;
}

static Fix imv[3][3] = {
/* 3x3 identity */
{ 1.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 1.0 } };

static Fix imt[3] =
/* zero translation */
{ 0.0, 0.0, 0.0 };

/*const MATRIX_f identity_matrix_f(imv, imt);

MATRIX_f::MATRIX_f() {}*/

const MATRIX_f identity_matrix_f = {
	{
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 }
	},
	{ 0.0, 0.0, 0.0 }
};

Fix vector_length_f(Fix x, Fix y, Fix z)
{
	return sqrt(fixToDouble(x*x + y*y + z*z));
}

void get_y_rotate_matrix_f(MATRIX_f *m, double r)
{
	double c, s;
	ASSERT(m);

	SINCOS(r, s, c);
	*m = identity_matrix_f;

	m->v[0][0] = c;
	m->v[0][2] = s;

	m->v[2][0] = -s;
	m->v[2][2] = c;
}

void get_translation_matrix_f(MATRIX_f *m, Fix x, Fix y, Fix z)
{
	ASSERT(m);
	*m = identity_matrix_f;

	m->t[0] = x;
	m->t[1] = y;
	m->t[2] = z;
}

void qsort (void  *base,
						size_t nel,
						size_t width,
						int (*comp)(const void *, const void *))
{
	size_t wgap, i, j, k;
	char tmp;

	if ((nel > 1) && (width > 0)) {
		ASSERT( nel <= ((size_t)(-1)) / width ); /* check for overflow */
		wgap = 0;
		do {
			wgap = 3 * wgap + 1;
		} while (wgap < (nel-1)/3);
		/* From the above, we know that either wgap == 1 < nel or */
		/* ((wgap-1)/3 < (int) ((nel-1)/3) <= (nel-1)/3 ==> wgap <  nel. */
		wgap *= width;			/* So this can not overflow if wnel doesn't. */
		nel *= width;			/* Convert nel to 'wnel' */
		do {
			i = wgap;
			do {
				j = i;
				do {
					register char *a;
					register char *b;

					j -= wgap;
					a = j + ((char *)base);
					b = a + wgap;
					if ( (*comp)(a, b) <= 0 ) {
						break;
					}
					k = width;
					do {
						tmp = *a;
						*a++ = *b;
						*b++ = tmp;
					} while ( --k );
				} while (j >= wgap);
				i += width;
			} while (i < nel);
			wgap = (wgap - width)/3;
		} while (wgap);
	}
}

void apply_matrix_f(const MATRIX_f *m, const myVertex* in, myVertex* out)
{
#define CALC_ROW(n) (in->x * m->v[(n)][0] + in->y * m->v[(n)][1] + in->z * m->v[(n)][2] + m->t[(n)])
	out->x = CALC_ROW(0);
	out->y = CALC_ROW(1);
	out->z = CALC_ROW(2);
#undef CALC_ROW
}

void clearScreen() {
	maSetColor(0);
	maFillRect(0, 0, SCREEN_W, SCREEN_H);
}

void polygon(int vertices, const int *points, int color) {
	maSetColor(color);
	maFillPolygon((const MAPoint2d*)points, vertices);
}


#define MAKE_ROTATION_f(x, y, z)                \
   Fix sin_x, cos_x;				\
   Fix sin_y, cos_y;				\
   Fix sin_z, cos_z;				\
   Fix sinx_siny, cosx_siny;			\
						\
   SINCOS(x, sin_x, cos_x);		\
   SINCOS(y, sin_y, cos_y);		\
   SINCOS(z, sin_z, cos_z);		\
						\
   sinx_siny = sin_x * sin_y;			\
   cosx_siny = cos_x * sin_y;

#define R00_f (cos_y * cos_z)
#define R10_f ((sinx_siny * cos_z) - (cos_x * sin_z))
#define R20_f ((cosx_siny * cos_z) + (sin_x * sin_z))

#define R01_f (cos_y * sin_z)
#define R11_f ((sinx_siny * sin_z) + (cos_x * cos_z))
#define R21_f ((cosx_siny * sin_z) - (sin_x * cos_z))

#define R02_f (-sin_y)
#define R12_f (sin_x * cos_y)
#define R22_f (cos_x * cos_y)

void get_transformation_matrix_f(MATRIX_f *m, Fix scale,
																 double xrot, double yrot, double zrot, Fix x, Fix y, Fix z)
{
   MAKE_ROTATION_f(xrot, yrot, zrot);
   ASSERT(m);

   m->v[0][0] = R00_f * scale;
   m->v[0][1] = R01_f * scale;
   m->v[0][2] = R02_f * scale;

   m->v[1][0] = R10_f * scale;
   m->v[1][1] = R11_f * scale;
   m->v[1][2] = R12_f * scale;

   m->v[2][0] = R20_f * scale;
   m->v[2][1] = R21_f * scale;
   m->v[2][2] = R22_f * scale;

   m->t[0] = x;
   m->t[1] = y;
   m->t[2] = z;
}
