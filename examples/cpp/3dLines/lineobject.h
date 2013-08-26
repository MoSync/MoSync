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
