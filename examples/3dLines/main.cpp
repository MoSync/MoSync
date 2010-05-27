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
#include <maprofile.h>
#include <conprint.h>
#include <MAP/MemoryMgr.h>

#include "main.h"
#include "MAHeaders.h"
#include "lineobject.h"
#include "Util.h"

/**
 * The size of the image.
 */
#define DRAWIMAGESIZE 50

#define FRAMES 1000
#define FPS 50
#define MS_PER_FRAME (1000/FPS)

ObjectWidget::ObjectWidget(int x, int y, int width, int height, Widget *parent)
	: Widget(x, y, width, height, parent) {
	init();
	Environment::getEnvironment().addTimer(this, MS_PER_FRAME, FRAMES+1);
}

ObjectWidget::~ObjectWidget() {
}


/**
  * \brief A function that generates a line object cube.
  * \param obj Reference to the LineObject object in which the cube should be built.
  * \param size The size of the generated cube.
  */
void ObjectWidget::generateCube(LineObject* obj, int size) {
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

/**
 * \brief Function that initializes the example.
 */
void ObjectWidget::init() {
	mode = 0;

	/// Create the image in which the cube will be drawn, to draw multiple versions of it.
	maCreateDrawableImage(1, DRAWIMAGESIZE, DRAWIMAGESIZE);

	initTrigTables();

	/// Create the line object, which will represent the cube.
	lineObject = new LineObject(8, 12, 0xffffff);

	/// Generate the cube.
	generateCube(lineObject, 7000);
}

/**
 * \brief Function of the example that draws the single object..
 */
void ObjectWidget::drawOne(int time) {
	/// Set the position of the cube.
	lineObject->setPosition(-20000, 0, 100000);

	/// Set the following draw calls to draw to the screen (draw target 0).
	maSetDrawTarget(0);

	/// Set the following draw calls to use color 0x181020 in format rgb888.
	maSetColor(0x181020);

	/// Clear the screen.
	maFillRect(0, 0, getWidth(), getHeight());

	/// Set the rotation of the cube.
	lineObject->setRotation((SIN(time/60)*TRIG_LOOKUP_MAGIC)>>TRIG_LOOKUP_BITS,
		              (SIN(time/50)*TRIG_LOOKUP_MAGIC)>>TRIG_LOOKUP_BITS,
					  (COS(time/40)*TRIG_LOOKUP_MAGIC)>>TRIG_LOOKUP_BITS);

	/// Transform the cube.
	lineObject->transform();

	/// Render the cube, the parts behind the sphere.
	lineObject->render(getWidth(), getHeight(),100000, 1000000, 0, 0);

	/// Get the image dimensions of the sphere.
	MAExtent e = maGetImageSize(R_SPRITE);

	/// Draw the sphere image.
	maDrawImage(R_SPRITE, (getWidth()>>1)-70, (getHeight()>>1)-((EXTENT_Y(e))>>1));

	/// Render the cube, the parts in front of the sphere.
	lineObject->render(getWidth(), getHeight(),0, 100000, 0, 0);

	/// Draw the logo image.
	maDrawImage(R_LOGO, (getWidth()>>1)-20, (getHeight()>>1)-30);
}

/**
 * \brief Function of the example that draws many objects..
 */
void ObjectWidget::drawLots(int time) {
	/// Set the position of the cube.
	lineObject->setPosition(0, 0, 140000);

	/// Set the following draw calls to draw to the drawable image created (draw target 1).
	maSetDrawTarget(1);

	/// Set the following draw calls to use color 0x181020 in format rgb888.
	maSetColor(0x181020);

	/// Clear the drawable image.
	maFillRect(0, 0, DRAWIMAGESIZE, DRAWIMAGESIZE);

	/// Set the rotation of the cube.
	lineObject->setRotation((SIN(time/20)*TRIG_LOOKUP_SIZE)>>TRIG_LOOKUP_BITS,
		             (SIN(time/30)*TRIG_LOOKUP_SIZE)>>TRIG_LOOKUP_BITS,
					 (COS(time/40)*TRIG_LOOKUP_SIZE)>>TRIG_LOOKUP_BITS);

	/// Transform the cube.
	lineObject->transform();

	/// Render the cube, the parts behind the sphere.
	lineObject->render(DRAWIMAGESIZE, DRAWIMAGESIZE,140000, 1000000, 0, 0);

	/// Get the image dimensions of the sphere.
	MAExtent e = maGetImageSize(R_SPRITE);

	/// Draw the sphere image.
	maDrawImage(R_SPRITE, (DRAWIMAGESIZE>>1)-((EXTENT_X(e))>>1), (DRAWIMAGESIZE>>1)-((EXTENT_Y(e))>>1));

	/// Render the cube, the parts in front of the sphere.
	lineObject->render(DRAWIMAGESIZE, DRAWIMAGESIZE,0, 140000, 0, 0);


	/// Set the following draw calls to draw to the screen (draw target 0).
	maSetDrawTarget(0);

	int numX = (getWidth()/DRAWIMAGESIZE);
	int numY = (getHeight()/DRAWIMAGESIZE);

	int xOffs = (getWidth()-((numX)*DRAWIMAGESIZE))>>1;
	int yOffs = (getHeight()-((numY)*DRAWIMAGESIZE))>>1;

	/// Draw the drawable image to the screen, several times using nested loops.
	for(int j = 0; j < numY; j++) {
		for(int i = 0; i < numX; i++) {
			MARect srcRect = {0, 0, DRAWIMAGESIZE, DRAWIMAGESIZE};
			MAPoint2d dstPoint = {xOffs+i*DRAWIMAGESIZE,yOffs+j*DRAWIMAGESIZE};
			maDrawImageRegion(1, &srcRect, &dstPoint, ((i+j)*(i-j))&0x7);
		}
	}
}

void ObjectWidget::drawWidget() {
}

void ObjectWidget::runTimerEvent() {
	/// Choose drawing function depending on mode.
	if(mode) {
		drawOne(maGetMilliSecondCount()*2);
	} else {
		drawLots(maGetMilliSecondCount()*2);
	}

	/// Updates the screen
	maUpdateScreen();

	/// Keep the backlight alive.
	maResetBacklight();
}

MainScreen::MainScreen(MyMoblet *moblet) {
	mMoblet = moblet;

	layout = new Layout(0, 0, gScreenWidth, gScreenHeight, NULL, 1, 2);

	softKeys = createSoftKeyBar(30, "Toggle", "Exit");

	objectWidget = new ObjectWidget(0, 0, gScreenWidth, gScreenHeight-softKeys->getHeight(), layout);
	objectWidget->setDrawBackground(false);

	layout->add(softKeys);

	this->setMain(layout);
}

MainScreen::~MainScreen() {
	delete layout;
}

void MainScreen::keyPressEvent(int keyCode, int nativeCode) {
	switch(keyCode) {
	case MAK_FIRE:
	case MAK_SOFTLEFT:
		{
		objectWidget->toggleMode();
		}
		break;
	case MAK_SOFTRIGHT:
		mMoblet->closeEvent();
		mMoblet->close();
		break;
	}
}

void MainScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);
	if(softKeys->contains(p)) {
		if(softKeys->getChildren()[0]->contains(p)) {
			keyPressEvent(MAK_SOFTLEFT, 0);
		}
		else if(softKeys->getChildren()[1]->contains(p)) {
			keyPressEvent(MAK_SOFTRIGHT, 0);
		}
	} else {
		objectWidget->toggleMode();
	}
}

void MainScreen::pointerReleaseEvent(MAPoint2d point) {
}

void MyMoblet::keyPressEvent(int keyCode, int nativeCode) {
}

void MyMoblet::keyReleaseEvent(int keyCode, int nativeCode) {
}

void MyMoblet::closeEvent() {
	// do destruction here
	delete mainScreen;
}

MyMoblet::MyMoblet() {
	gFont = new MAUI::Font(RES_FONT);
	gSkin = new WidgetSkin(RES_SELECTED, RES_SELECTED, 3, 18, 10, 11, true, true);
	Engine& engine = Engine::getSingleton();
	engine.setDefaultFont(gFont);
	engine.setDefaultSkin(gSkin);

	MAExtent screenSize = maGetScrSize();
	gScreenWidth = EXTENT_X(screenSize);
	gScreenHeight = EXTENT_Y(screenSize);

	mainScreen = new MainScreen(this);
	mainScreen->show();
}

extern "C" int MAMain() {
	Moblet::run( newobject( MyMoblet, new MyMoblet( ) ) );
	return 0;
}
