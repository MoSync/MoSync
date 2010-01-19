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

/** \file main.cpp
*
* This file contains the main example program source.
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#include <ma.h>
#include <mastdlib.h>
#include <conprint.h>
#include "MAHeaders.h"
#include "lineobject.h"

/**
 * The size of the image.
 */
#define DRAWIMAGESIZE 50

/**
  * \brief A function that generates a line object cube.
  * \param obj Reference to the LineObject object in which the cube should be built.
  * \param size The size of the generated cube.
  */
static void generateCube(LineObject* obj, int size) {

	/// Static array containing the "unit square"
	int points[8][3] = {
		{-1, 1,-1},
		{ 1, 1,-1},
		{ 1, 1, 1},
		{-1, 1, 1},
		{-1,-1,-1},
		{ 1,-1,-1},
		{ 1,-1, 1},
		{-1,-1, 1}
	};

	/// Static array containing the line indecies of the previously declared square.
	int lines[12][2] = {
		{0, 1},
		{1, 2},
		{2, 3},
		{3, 0},
		{4, 5},
		{5, 6},
		{6, 7},
		{7, 4},
		{0, 4},
		{3, 7},
		{2, 6},
		{1, 5}
	};

	/// Add the vertices.
	for(int i = 0; i < 8; i++)
		obj->addVertex(points[i][0]*size, points[i][1]*size, points[i][2]*size);

	/// Add the lines.
	for(int i = 0; i < 12; i++)
		obj->addLine(lines[i][0], lines[i][1]);

}

LineObject* gObj;
int gScreenWidth;
int gScreenHeight;

/**
 * \brief Function that initializes the example.
 */
static void init() {
	/// Create the image in which the cube will be drawn, to draw multiple versions of it.
	maCreateDrawableImage(1, DRAWIMAGESIZE, DRAWIMAGESIZE);

	initTrigTables();

	/// Create the line object, which will represent the cube.
	gObj = new LineObject(8, 12, 0xffffff);

	/// Generate the cube.
	generateCube(gObj, 7000);

	/// Get screen dimensions.
	MAExtent size = maGetScrSize();

	/// Extract the screen width
	gScreenWidth = EXTENT_X(size);

	/// Extract the screen height
	gScreenHeight = EXTENT_Y(size);
}

/**
 * \brief Function of the example that draws the single object..
 */
static void drawOne(int time) {
	/// Set the position of the cube.
	gObj->setPosition(-20000, 0, 100000);

	/// Set the following draw calls to draw to the screen (draw target 0).
	maSetDrawTarget(0);

	/// Set the following draw calls to use color 0x181020 in format rgb888.
	maSetColor(0x181020);

	/// Clear the screen.
	maFillRect(0, 0, gScreenWidth, gScreenHeight);

	/// Set the rotation of the cube.
	gObj->setRotation((SIN(time/60)*TRIG_LOOKUP_MAGIC)>>TRIG_LOOKUP_BITS,
		              (SIN(time/50)*TRIG_LOOKUP_MAGIC)>>TRIG_LOOKUP_BITS,
					  (COS(time/40)*TRIG_LOOKUP_MAGIC)>>TRIG_LOOKUP_BITS);

	/// Transform the cube.
	gObj->transform();

	/// Render the cube, the parts behind the sphere.
	gObj->render(gScreenWidth, gScreenHeight,100000, 1000000, 0, 0);

	/// Get the image dimensions of the sphere.
	MAExtent e = maGetImageSize(R_SPRITE);

	/// Draw the sphere image.
	maDrawImage(R_SPRITE, (gScreenWidth>>1)-70, (gScreenHeight>>1)-((EXTENT_Y(e))>>1));

	/// Render the cube, the parts in front of the sphere.
	gObj->render(gScreenWidth, gScreenHeight,0, 100000, 0, 0);

	/// Draw the logo image.
	maDrawImage(R_LOGO, (gScreenWidth>>1)-20, (gScreenHeight>>1)-30);
}

/**
 * \brief Function of the example that draws many objects..
 */
static void drawLots(int time) {
	/// Set the position of the cube.
	gObj->setPosition(0, 0, 140000);

	/// Set the following draw calls to draw to the drawable image created (draw target 1).
	maSetDrawTarget(1);

	/// Set the following draw calls to use color 0x181020 in format rgb888.
	maSetColor(0x081020);

	/// Clear the drawable image.
	maFillRect(0, 0, DRAWIMAGESIZE, DRAWIMAGESIZE);

	/// Set the rotation of the cube.
	gObj->setRotation((SIN(time/20)*TRIG_LOOKUP_SIZE)>>TRIG_LOOKUP_BITS,
		             (SIN(time/30)*TRIG_LOOKUP_SIZE)>>TRIG_LOOKUP_BITS,
					 (COS(time/40)*TRIG_LOOKUP_SIZE)>>TRIG_LOOKUP_BITS);

	/// Transform the cube.
	gObj->transform();

	/// Render the cube, the parts behind the sphere.
	gObj->render(DRAWIMAGESIZE, DRAWIMAGESIZE,140000, 1000000, 0, 0);

	/// Get the image dimensions of the sphere.
	MAExtent e = maGetImageSize(R_SPRITE);

	/// Draw the sphere image.
	maDrawImage(R_SPRITE, (DRAWIMAGESIZE>>1)-((EXTENT_X(e))>>1), (DRAWIMAGESIZE>>1)-((EXTENT_Y(e))>>1));

	/// Render the cube, the parts in front of the sphere.
	gObj->render(DRAWIMAGESIZE, DRAWIMAGESIZE,0, 140000, 0, 0);


	/// Set the following draw calls to draw to the screen (draw target 0).
	maSetDrawTarget(0);

	int numX = (gScreenWidth/DRAWIMAGESIZE);
	int numY = (gScreenHeight/DRAWIMAGESIZE);

	int xOffs = (gScreenWidth-((numX)*DRAWIMAGESIZE))>>1;
	int yOffs = (gScreenHeight-((numY)*DRAWIMAGESIZE))>>1;

	/// Draw the drawable image to the screen, several times using nested loops.
	for(int j = 0; j < numY; j++) {
		for(int i = 0; i < numX; i++) {
			MARect srcRect = {0, 0, DRAWIMAGESIZE, DRAWIMAGESIZE};
			MAPoint2d dstPoint = {xOffs+i*DRAWIMAGESIZE,yOffs+j*DRAWIMAGESIZE};
			maDrawImageRegion(1, &srcRect, &dstPoint, ((i+j)*(i-j))&0x7);
		}
	}
}

extern "C" {
/**
 * \brief The entry point.
 */
int MAMain()
{
	int mode = 1;

	init();

	while(1) {

		/// Get any available events.
		/// If MAK_FIRE is pressed, change mode.
		/// On Close event or MAK_0 press, close program.
		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_FIRE) {
				mode ^= 1;
				maSetDrawTarget(0);
				maSetColor(0x081020);
				maFillRect(0, 0, gScreenWidth, gScreenHeight);

			} else if(event.type == EVENT_TYPE_POINTER_PRESSED) {
				mode = 0;
			} else if(event.type == EVENT_TYPE_POINTER_RELEASED) {
				mode = 1;
			} else if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			}
		}

		/// Choose drawing function depending on mode.
		if(mode)
			drawOne(maGetMilliSecondCount()*2);
		else
			drawLots(maGetMilliSecondCount()*2);

		/// Updates the screen
		maUpdateScreen();

		/// Keep the backlight alive.
		maResetBacklight();
	}

	delete gObj;

	return 0;
}
}
