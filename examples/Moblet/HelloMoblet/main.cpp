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


#include <ma.h>
#include <mastdlib.h>
#include <conprint.h>
#include <maprofile.h>
#include <MAP/MemoryMgr.h>

#include "main.h"
#include "util.h"

// this is how many pixels will be traversed each timer tick
#define SPEED 1
#define FRAMES 1000
#define FPS 50
#define MS_PER_FRAME (1000/FPS)


ObjectWidget::ObjectWidget(int x, int y, int width, int height, Widget *parent)
	: Widget(x, y, width, height, parent), mX(30), mY(40) {
	// set the text
	text = "Hello world!";
	draw(mX, mY);
}

ObjectWidget::~ObjectWidget() {
}

// This is the function that draws a frame, it is made as a seperate function
// because both the constructor and the timer callback needs to call it.
void ObjectWidget::draw(int x, int y) {
	MAExtent stringSize = maGetTextSize(text);
	int stringSizeX = EXTENT_X(stringSize);
	int stringSizeY = EXTENT_Y(stringSize);
	if((x + stringSizeX) > getWidth()) x = getWidth() - stringSizeX;
	if((y + stringSizeY) > getHeight()) y = getHeight() - stringSizeY;
	if(x < 0) x = 0;
	if(y < 0) y = 0;

	mX = x;
	mY = y;

	maSetColor(0);
	maFillRect(0, 0, getWidth(), getHeight());
	maSetColor(0xffffff);
	maDrawText(mX, mY, text);

	/// Updates the screen
	maUpdateScreen();

	/// Keep the backlight alive.
	maResetBacklight();
}

void ObjectWidget::drawWidget() {
}

MainScreen::MainScreen(MyMoblet *moblet) : dirx(0), diry(0) {
	mMoblet = moblet;

	layout = new Layout(0, 0, gScreenWidth, gScreenHeight, NULL, 1, 2);

	softKeys = createSoftKeyBar(30, "", "Exit");

	objectWidget = new ObjectWidget(0, 0, gScreenWidth, gScreenHeight-softKeys->getHeight(), layout);
	objectWidget->setDrawBackground(false);

	layout->add(softKeys);

	this->setMain(layout);
}

MainScreen::~MainScreen() {
	delete layout;
}

void MainScreen::runTimerEvent() {
	Point point = objectWidget->getPosition();
	point.x += dirx * SPEED;
	point.y += diry * SPEED;
	objectWidget->draw(point.x, point.y);
}

void MainScreen::keyPressEvent(int keyCode, int nativeCode) {
	switch(keyCode) {
	case MAK_LEFT:
		dirx = -1;
		break;
	case MAK_RIGHT:
		dirx = 1;
		break;
	case MAK_UP:
		diry = -1;
		break;
	case MAK_DOWN:
		diry = 1;
		break;
	case MAK_SOFTRIGHT:
		mMoblet->closeEvent();
		mMoblet->close();
		return;
	default:
		return;
	}

	Environment::getEnvironment().addTimer(this, MS_PER_FRAME, FRAMES+1);
}

void MainScreen::keyReleaseEvent(int keyCode, int nativeCode) {
	// only reset the directions if it is the current direction
	switch(keyCode) {
		case MAK_LEFT:
			if(dirx == -1)
				dirx = 0;
			break;
		case MAK_RIGHT:
			if(dirx ==  1)
				dirx = 0;
			break;
		case MAK_UP:
			if(diry == -1)
				diry = 0;
			break;
		case MAK_DOWN:
			if(diry ==  1)
				diry = 0;
			break;
		default:
			return;
	}

	// remove the animation timer if nothing is still moving
	if(dirx == 0 && diry == 0)
		Environment::getEnvironment().removeTimer(this);
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
		objectWidget->draw(point.x, point.y);
	}
}

void MainScreen::pointerMoveEvent(MAPoint2d point) {
	objectWidget->draw(point.x, point.y);
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
