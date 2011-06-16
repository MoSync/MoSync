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

/** \file lineobject.cpp
*
* This file contains an implementation LineObject of the class
* Drawable, for drawing objects built up of lines.
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#include "lineobject.h"
#include <ma.h>

/**
 * Create line object, specifying the amount of vertices, lines and which color the object should have.
 */
LineObject::LineObject(int numVertices, int numLines, int color) :
	Drawable(numVertices),
	mNumLines(numLines),
	mCurLines(0),
	mLines(new Line[mNumLines]),
	mColor(color)
	{
	}


/**
 * Render line object.
 */
void LineObject::render(int width, int height, int znear, int zfar, int xOff, int yOff) {

	int thickness = 2;
	maSetColor(mColor);

	for(int i = 0; i < mCurLines; i++) {
		Vector& p1 = mTransformed[mLines[i].i1];
		Vector& p2 = mTransformed[mLines[i].i2];
		int az = (p1.mZ+p2.mZ)>>1;
		if(az<znear) continue;
		if(az>zfar) continue;
		int x1,y1;
		int x2,y2;
		PROJECT_POINT(x1, y1, p1, width, height);
		PROJECT_POINT(x2, y2, p2, width, height);
		for(int j = thickness-1; j >= 0; j--) {
			maLine(x1-j, y1, x2-j, y2);
			maLine(x1+j, y1, x2+j, y2);
			maLine(x1, y1-j, x2, y2-j);
			maLine(x1, y1+j, x2, y2+j);
		}
	}
}

/**
 * Add line to line object.
 */
void LineObject::addLine(int i1, int i2) {
	if(mCurLines<mNumLines)
		mLines[mCurLines++] = Line(i1, i2);
}
