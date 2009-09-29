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

#ifndef ALLEGRO_H
#define ALLEGRO_H

#include <maassert.h>

#define ASSERT MAASSERT

#define USE_DOUBLE


//******************************************************************************
//a 32-bit fixed-point number class
//******************************************************************************

#ifdef USE_DOUBLE
typedef double Fix;
inline int fixToInt(Fix f) { return (int)f; }
inline double fixToDouble(Fix f) { return (double)f; }
#else

typedef long long i64;

#define FIX_SHIFT 16
#define FIX_MUL (1 << FIX_SHIFT)
class Fix {
public:
	Fix() {}
	Fix(double x) {
		m = (int)(x * double(FIX_MUL) + (x < 0 ? -0.5 : 0.5));
	}
	Fix(const Fix& f) : m(f.m) {}

	Fix& operator=(Fix f) { m = f.m; return *this; }
	Fix& operator+=(Fix f) { m += f.m; return *this; }
	Fix& operator-=(Fix f) { m -= f.m; return *this; }
	Fix& operator/=(Fix f) { m <<= 8; f.m >>= 8; m /= f.m; return *this; }

	//m = (int)(((i64)(m)) * ((i64)(f))); return *this; }
	Fix& operator*=(Fix f) { m >>= 8; f.m >>= 8; m *= f.m; return *this; }

	Fix operator-() { Fix f = *this; f.m = -f.m; return f; }

	friend Fix operator+(Fix f1, Fix f2) { f1 += f2; return f1; }
	friend Fix operator-(Fix f1, Fix f2) { f1 -= f2; return f1; }
	friend Fix operator*(Fix f1, Fix f2) { f1 *= f2; return f1; }
	friend Fix operator/(Fix f1, Fix f2) { f1 /= f2; return f1; }

	friend bool operator<(Fix f1, Fix f2) { return f1.m < f1.m; }
	friend bool operator>(Fix f1, Fix f2) { return f1.m > f1.m; }

	friend int fixToInt(Fix f) { return f.m >> 16; }
	friend Fix itofix(int i) { Fix f; f.m = i << FIX_SHIFT; return f; }
	friend double fixToDouble(Fix f) { return double(f.m) / FIX_MUL; }
private:
	int m;
};
#endif	//USE_DOUBLE

//******************************************************************************
//Math routines
//******************************************************************************

struct vec3 {
	Fix x, y, z;
};

typedef struct vec3 myVertex;

//a 4x4 matrix using Fix
struct MATRIX_f {
	//MATRIX_f();
	//MATRIX_f(Fix _v[3][3], Fix _t[3]) { memcpy(v, _v, sizeof(v)); memcpy(t, _t, sizeof(t)); }
	Fix v[3][3];
	Fix t[3];
};

void matrix_mul_f(const MATRIX_f* a1, const MATRIX_f* a2, MATRIX_f* out);
void apply_matrix_f(const MATRIX_f *m, const myVertex* in, myVertex* out);
void get_camera_matrix_f(MATRIX_f *m, Fix x, Fix y, Fix z,
												 Fix xfront, Fix yfront, Fix zfront,
												 Fix xup, Fix yup, Fix zup, double fov, Fix aspect);
void get_translation_matrix_f(MATRIX_f *out, Fix x, Fix y, Fix z);
void get_x_rotate_matrix_f(MATRIX_f *m, double r);
void get_y_rotate_matrix_f(MATRIX_f *out, double rotation);
void get_scaling_matrix_f(MATRIX_f *m, Fix x, Fix y, Fix z);
void get_transformation_matrix_f(MATRIX_f *m, Fix scale,
																 double xrot, double yrot, double zrot, Fix x, Fix y, Fix z);

extern const MATRIX_f identity_matrix_f;
extern int SCREEN_W, SCREEN_H;

void persp_project_f(myVertex* v);
void set_projection_viewport(int x, int y, int w, int h);

void normalize_vector_f(Fix *x, Fix *y, Fix *z);
void cross_product_f(Fix x1, Fix y1, Fix z1, Fix x2, Fix y2, Fix z2,
										 Fix *xout, Fix *yout, Fix *zout);
Fix vector_length_f(Fix x, Fix y, Fix z);

inline Fix dot_product_f(Fix x1, Fix y_1, Fix z1, Fix x2, Fix y2, Fix z2)
{
	return (x1 * x2) + (y_1 * y2) + (z1 * z2);
}

#define BIN2RAD(d) (((d) * M_PI) / 128)
#define SINCOS(r, s, c) { s = sin(BIN2RAD(r)); c = cos(BIN2RAD(r)); }

//******************************************************************************
//graphics
//******************************************************************************

void polygon(int vertices, const int *points, int color);
void clearScreen();

//******************************************************************************
//other
//******************************************************************************
void qsort (void  *base,
						size_t nel,
						size_t width,
						int (*comp)(const void *, const void *));

#endif	//ALLEGRO_H
