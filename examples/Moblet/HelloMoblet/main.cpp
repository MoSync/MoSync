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
#include <MAUtil/Moblet.h>

using namespace MAUtil;

/**
 * Simple Moblet example that shows how timers and keyevents work.
 * Displays a "hello world" text which is movable using the joystick.
 */

// this is how many pixels will be traversed each timer tick
#define SPEED 1

class MyMoblet : public Moblet, TimerListener {
public:
	MyMoblet() : dirx(0), diry(0), x(30), y(40) {
		// set the text
		text = "Hello world!"; 

		// retrieve and save the screen dimensions
		MAExtent screenSize = maGetScrSize();
		screenWidth = EXTENT_X(screenSize);
		screenHeight = EXTENT_Y(screenSize);

		// draw initially one time
		draw();
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// set the current direction
		switch(keyCode) {
			case MAK_LEFT:	dirx = -1;	break;
			case MAK_RIGHT: dirx = 1;	break;
			case MAK_UP:	diry = -1;	break;
			case MAK_DOWN:	diry = 1;	break;
			case MAK_SOFTRIGHT:
			case MAK_0:
				maExit(0);
			default:
				return;
		}

		// add an infinite timer to update the animation until the key is released
		// update the animation in 50 fps (each 20:th millisecond)
		addTimer(this, 20, -1);
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
		// only reset the directions if it is the current direction
		switch(keyCode) {
			case MAK_LEFT:	if(dirx == -1) dirx = 0; break;
			case MAK_RIGHT: if(dirx ==  1) dirx = 0; break;
			case MAK_UP:	if(diry == -1) diry = 0; break;
			case MAK_DOWN:	if(diry ==  1) diry = 0; break;
			default:
				return;
		}

		// remove the animation timer if nothing is still moving
		if(dirx == 0 && diry == 0)
			removeTimer(this);
	}

	virtual void pointerPressEvent(MAPoint2d p) {
		x = p.x;
		y = p.y;
		draw();
	}

	virtual void pointerMoveEvent(MAPoint2d p) {
		x = p.x;
		y = p.y;
		draw();
	}

	virtual void pointerReleaseEvent(MAPoint2d p) {
	}

	// This is the function that draws a frame, it is made as a seperate function 
	// because both the constructor and the timer callback needs to call it.
	void draw() {
		maSetColor(0);
		maFillRect(0, 0, screenWidth, screenHeight);
		maSetColor(0xffffff);
		maDrawText(x, y, text);
		maUpdateScreen();
	}

	// Timer callback that checks if the position is inside the boundaries. 
	// It then draws the frame and updates the position.
	void runTimerEvent() {
		MAExtent stringSize = maGetTextSize(text);
		int stringSizeX = EXTENT_X(stringSize);
		int stringSizeY = EXTENT_Y(stringSize);
		if((x + stringSizeX) > screenWidth)  x = screenWidth - stringSizeX;
		if((y + stringSizeY) > screenHeight) y = screenHeight - stringSizeY;
		if(x < 0) x = 0;
		if(y < 0) y = 0;

		draw();

		x += dirx * SPEED;
		y += diry * SPEED;
	}

private:
	const char *text;
	int dirx, diry;
	int x, y;
	int screenWidth, screenHeight;
};

extern "C" int MAMain() {
	MyMoblet moblet;
	MyMoblet::run(&moblet);
	return 0;
}
