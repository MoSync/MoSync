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

/** \file drawable.h
*
* This file contains the class Drawable, representing a vertex model of any drawable 3-dimensional object.
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include "maths.h"

/**
 * Project a 3d point to a 2d screen plane, specifying its width and height
 */
#define PROJECT_POINT(sx, sy, vec, w, h) {\
	int recip = 0xffffff/vec.mZ; \
	sx=(((vec.mX)*recip)>>16)+(w>>1); \
	sy=(((vec.mY)*recip)>>16)+(h>>1); \
}

/**
 * Class representing a drawable 3-dimensional object.
 */
class Drawable {
public:

	/**
	 * \brief Create drawable object.
	 * \param numVertices The number of vertices.
	 */
	Drawable(int numVertices);
	virtual ~Drawable();

	/**
	 * \brief Add a vertex to the object
	 * \param x x-coordinate.
	 * \param y y-coordinate.
	 * \param z z-coordinate.
	 */
	void addVertex(int x, int y, int z);

	/**
	 * \brief Set position of the object.
	 * \param x x-coordinate.
	 * \param y y-coordinate.
	 * \param z z-coordinate.
	 */
	void setPosition(int x, int y, int z);

	/**
	 * \brief Set rotation of the object
	 * \param x x-axis angle.
	 * \param y y-axis angle.
	 * \param z z-axis angle.
	 */
	void setRotation(int x, int y, int z);

	/**
	 * \brief Set scale of the object.
	 * \param x x-axis scale factor.
	 * \param y y-axis scale factor.
	 * \param z z-axis scale factor.
	 */
	void setScale(int x, int y, int z);

	/**
	 * \brief Set scale of the object.
	 * \return The number of vertices.
	 */
	int getNumVertices();

	/**
 	 * \brief Transforms the object and stores the new coordinates for rendering.
	 * \note Call this before you call render if you want to apply transformations.
	 */
	void transform();

	/**
	 * \brief Call this function when you want to render the drawable object.
	 * \note Each implementation needs to overload this function for different drawing methods.
	 */
	virtual void render(int width, int height, int znear, int zfar, int xOff, int yOff) = 0;

protected:
	/// The current vertex to which addVertex should add a vertex.
	int mCurVertex;

	/// The number of vertices.
	int mNumVertices;

	/// The array holding the untransformed, original, vertices.
	Vector *mOriginal;

	/// The array holding the transformed vertices.
	Vector *mTransformed;

	/// The position.
	Vector mPosition;

	/// The rotation.
	Vector mRotation;

	/// The scale.
	Vector mScale;
};

#endif
