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
