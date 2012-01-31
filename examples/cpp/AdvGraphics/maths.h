/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/
/** \file maths.h
*
* This file contains mathematical support functions and macros.
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#ifndef _MATHS_H_
#define _MATHS_H_

/**
 * An approximation of pi to 5 decimals.
 */
#define PI 3.14159

/**
 * Class representing a vector in 3-space.
 */
class Vector {
public:
	/**
	 * \brief Create a vector.
	 */
	Vector();

	/**
	 * \brief Create a vector.
	 * \param x x-coordinate.
	 * \param y y-coordinate.
	 * \param z z-coordinate.
	 */
	Vector(int x, int y, int z);


	/// x-coordinate
	int mX;

	/// y-coordinate
	int mY;

	/// z-coordinate
	int mZ;
};

/**
 * The size of the sinus lookup table.
 */
#define TRIG_LOOKUP_SIZE 1024

/**
 * The number of bits that is used by each element in the sinus lookup table.
 */
#define TRIG_LOOKUP_BITS 10

/**
 * The sinus lookup table.
 */
extern int sinTable[TRIG_LOOKUP_SIZE];

/**
 * Returns a fixed point representation of sin(x) with TRIG_LOOKUP_BITS bits of precision.
 */
#define SIN(x) (sinTable[(x)&(TRIG_LOOKUP_SIZE-1)])

/**
 * Returns a fixed point representation of cos(x) with TRIG_LOOKUP_BITS bits of precision.
 */
#define COS(x) (sinTable[(x+(TRIG_LOOKUP_SIZE>>2))&(TRIG_LOOKUP_SIZE-1)])

/**
 * Initialize the lookup tables.
 */
void initTrigTables();

/**
 * \brief Rotate a vector around the x axis.
 * \param ang The angle to rotate with.
 * \param src The source vector.
 * \param dest The destination vector.
 */
inline void rotateX(int ang, Vector* src, Vector* dest) {
	// x rotation
	dest->mY = (src->mY*COS(ang) - src->mZ*SIN(ang))>>TRIG_LOOKUP_BITS;
	dest->mZ = (src->mY*SIN(ang) + src->mZ*COS(ang))>>TRIG_LOOKUP_BITS;
	dest->mX = src->mX;
}

/**
 * \brief Rotate a vector around the y axis.
 * \param ang The angle to rotate with.
 * \param src The source vector.
 * \param dest The destination vector.
 */
inline void rotateY(int ang, Vector* src, Vector* dest) {
	// y rotation
	dest->mZ = (src->mZ*COS(ang) - src->mX*SIN(ang))>>TRIG_LOOKUP_BITS;
	dest->mX = (src->mZ*SIN(ang) + src->mX*COS(ang))>>TRIG_LOOKUP_BITS;
	dest->mY = src->mY;
}

/**
 * \brief Rotate a vector around the z axis.
 * \param ang The angle to rotate with.
 * \param src The source vector.
 * \param dest The destination vector.
 */
inline void rotateZ(int ang, Vector* src, Vector* dest) {
	// z rotation
	dest->mX = (src->mX*COS(ang) - src->mY*SIN(ang))>>TRIG_LOOKUP_BITS;
	dest->mY = (src->mX*SIN(ang) + src->mY*COS(ang))>>TRIG_LOOKUP_BITS;
	dest->mZ = src->mZ;
}


#endif
