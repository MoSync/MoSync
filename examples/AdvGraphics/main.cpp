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
#include <maprofile.h>
#include <MAP/MemoryMgr.h>

#include "main.h"
#include "util.h"

#define RGBA(r, g, b, a) ((r)<<16)|((g)<<8)|((b))|((a)<<24)
#define FRAMES 1000
#define FPS 50
#define MS_PER_FRAME (1000/FPS)

class DynamicResources {
public:
	static MAHandle maCreateDrawableImage(int width, int height) {
		MAHandle h = maCreatePlaceholder();
		::maCreateDrawableImage(h, width, height);
		return h;
	}

	static MAHandle maCreateImageRaw(const void *src, MAExtent size, int alpha) {
		MAHandle h = maCreatePlaceholder();
		::maCreateImageRaw(h, src, size, alpha);
		return h;
	}
};


ObjectWidget::ObjectWidget(int x, int y, int width, int height, Widget *parent)
	: Widget(x, y, width, height, parent) {
	numImages = 0;
	Environment::getEnvironment().addTimer(this, MS_PER_FRAME, FRAMES+1);
}

ObjectWidget::~ObjectWidget() {
}

MAHandle ObjectWidget::generateImage(int width, int height, int r, int g, int b) {
	int *temp = new int[width*height];
	int *dst = temp;
	double longestLength;
	if(width<height)
		longestLength = width>>1;
	else
		longestLength = height>>1;

	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			double d = sqrt((double)((width>>1)-i)*((width>>1)-i) +
				((height>>1)-j)*((height>>1)-j));
			d /= longestLength;
			d = 1.0-d;
			if(d<0) d = 0;
			if(d>1) d = 1;
			unsigned char intensity = (unsigned char)(d*255.0);
			*dst++ = RGBA((intensity*r)>>8, (intensity*g)>>8, (intensity*b)>>8, intensity);
		}
	}

	MAHandle h = DynamicResources::maCreateImageRaw(temp, EXTENT(width, height), 1);
	delete temp;
	return h;
}

void ObjectWidget::createImage() {
	if(numImages+1>=MAX_IMAGES) return;
	images[numImages] = generateImage(32, 32, rand()&0xff, rand()&0xff, rand()&0xff);
	numImages++;
}

void ObjectWidget::drawImages() {
	double a = 0;
	double time = maGetMilliSecondCount()*0.001;
	int w = getWidth()>>1;
	int h = getHeight()>>1;
	for(int i = 0; i < numImages; i++) {
		int x = (int)(cos(a*1.5+time*1.1)*50.0) + w;
		int y = (int)(sin(a*1.09+time*1.2)*50.0) + h;
		MAExtent imageSize = maGetImageSize(images[i]);
		maDrawImage(images[i], x-(EXTENT_X(imageSize)>>1), y-(EXTENT_Y(imageSize)>>1));
		a+=1.0;
	}
}

void ObjectWidget::runTimerEvent() {
	drawImages();

	/// Updates the screen
	maUpdateScreen();

	/// Keep the backlight alive.
	maResetBacklight();
}

void ObjectWidget::drawWidget() {
}

MainScreen::MainScreen(MyMoblet *moblet) {
	mMoblet = moblet;

	layout = new Layout(0, 0, gScreenWidth, gScreenHeight, NULL, 1, 2);

	softKeys = createSoftKeyBar(30, "Spell", "Exit");

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
		objectWidget->createImage();
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
		objectWidget->createImage();
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
