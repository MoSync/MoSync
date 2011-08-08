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

/** \file lineobject.h
*
* This file contains an implementation LineObject of the class
* Drawable, for drawing objects built up of lines.
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#ifndef _LINEOBJECT_H_
#define _LINEOBJECT_H_

#include "drawable.h"
#include "maths.h"

/**
 * Struct representing a line, by two indecies to a vertex array.
 */
struct Line {
	/**
	 * Create a line object.
	 */
	Line() : i1(0), i2(0) {}

	/**
	 * \brief Create a line object.
	 * \param i1 Index to the vertex in which the line should start.
	 * \param i2 Index to the vertex in which the line should end.
	 */
	Line(int i1, int i2) : i1(i1), i2(i2) {}

	 /// Index to the vertex in which the line should start.
	int i1;

	 /// Index to the vertex in which the line should end.
	int i2;
};

/**
 * Class representing a drawable line object.
 */
class LineObject : public Drawable {
public:

	/**
	 * \brief Create line object.
	 * \param numVertices The number of vertices the object consist of.
	 * \param numLines The number of lines the object should be drawn with.
	 * \param color The color of the object.
	 */
	LineObject(int numVertices, int numLines, int color);

	/**
	 * \brief Render line object.
	 */
	void render(int width, int height, int znear, int zfar, int xOff, int yOff);

	/**
	 * \brief Add line to the drawable line object.
	 * \param i1 Index to the vertex in which the line should start.
	 * \param i2 Index to the vertex in which the line should end.
	 */
	void addLine(int i1, int i2);

protected:
	 /// The number of lines of the line object.
	int mNumLines;

	/// The current line, which should be when adding a line.
	int mCurLines;

	/// The array of lines.
	Line* mLines;

	/// Color for the lines.
	int mColor;
};

#endif
