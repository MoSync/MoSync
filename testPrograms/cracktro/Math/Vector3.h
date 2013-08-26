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

#ifndef _SE_MSAB_MATH_VECTOR3_H_
#define _SE_MSAB_MATH_VECTOR3_H_

/**
 * The Vector3 struct defines a three-dimensional 
 * euclidian vector which is associated with a set
 * of functions to manage and manipulate such vectors.
 *
 * 
 * @author      Patrick Broman
 * @version     %I%, %G%
 * @since       1.0
 */

typedef struct Vector3_t {
	double x,y,z;		
} Vector3;

static const Vector3 vec3_zero  = {0.0, 0.0, 0.0};
static const Vector3 vec3_xAxis = {1.0, 0.0, 0.0};
static const Vector3 vec3_yAxis = {0.0, 1.0, 0.0};
static const Vector3 vec3_zAxis = {0.0, 0.0, 1.0};

/**
 * Allocates heap memory for a Vector3 instance, and sets
 * its components to the specified values.
 *
 * @param  x  The x component of the vector
 * @param  y  The y component of the vector
 * @param  z  The z component of the vector
 * @return  a pointer to an instance of Vector3 with the specified components
 * @see Vector2
 *
 */

Vector3* Vec3_alloc(double x, double y, double z);

/**
 * Creates a local Vector3 instance on the stack
 * and returns it by value, setting its components
 * to the specified values.
 *
 * @param  x  The x component of the vector
 * @param  y  The y component of the vector
 * @param  z  The z component of the vector
 * @return an instance of a Vector3 with the specified components
 *
 */

Vector3 Vec3_create(double x, double y, double z);

/**
 * Adds two instances of Vector3 component-wise and
 * returns the resulting vector by value.
 *
 * @param  v1 A pointer to the left-hand vector
 * @param  v2 A pointer to the right-hand vector
 * @return The vector sum of the two vectors
 *
 */

Vector3 Vec3_add(const Vector3* v1, const Vector3* v2);

/**
 * Adds two instances of Vector3 component-wise and
 * returns the resulting vector by value.
 *
 * @param  v1 A pointer to the left-hand vector
 * @param  v2 A pointer to the right-hand vector
 * @return The vector sum of the two vectors
 *
 */

Vector3 Vec3_sub(const Vector3* v1, const Vector3* v2);

Vector3 Vec3_mul(const Vector3* v, double d);


double Vec3_magnitude(const Vector3* v);

Vector3* Vec3_normalize(Vector3* v);
Vector3 Vec3_toUnit(const Vector3* v);
	
double Vec3_dot(const Vector3* v1, const Vector3* v2);
double Vec3_angle(const Vector3* v1, const Vector3* v2);

Vector3 Vec3_cross(const Vector3* v1, const Vector3* v2);

char* Vec3_toString(const Vector3* v);

#endif
