/* Copyright (C) 2011 MoSync AB

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

#include <MAUtil/Moblet.h>
#include <MAUtil/Map.h>
#include <mavsprintf.h>

using namespace MAUtil;

class MyMoblet : public Moblet, public TimerListener {
public:
	MyMoblet() {
		// todo: initialize

		Environment::getEnvironment().addTimer(this, 30, -1);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
		// todo: handle key releases
	}

	Map<int, MAPoint2d> mTouches;


	/**
	* This function is called with a coordinate when a pointer is pressed.
	*/
	virtual void multitouchPressEvent(MAPoint2d p, int touchId) {
		mTouches[touchId] = p;
	}

	/**
	*  This function is called with a coordinate when a pointer is moved.
	*/
	virtual void multitouchMoveEvent(MAPoint2d p, int touchId) {
		mTouches[touchId] = p;
	}

	/**
	*  This function is called with a coordinate when a pointer is released.
	*/
	virtual void multitouchReleaseEvent(MAPoint2d p, int touchId) {
		mTouches.erase(touchId);
	}

	void runTimerEvent() {
		maSetColor(0);
		maFillRect(0, 0, EXTENT_X(maGetScrSize()), EXTENT_Y(maGetScrSize()));

		maSetColor(0xffffff);

		for(Map<int, MAPoint2d>::Iterator iterator = mTouches.begin(); iterator != mTouches.end(); iterator++) {
			MAPoint2d point = iterator->second;
			int touchId = iterator->first;

			char touchStr[16];
			sprintf(touchStr, "%d", touchId);

			maSetColor(0xff0000);
			maFillRect(point.x - 25, point.y - 25, 50, 50);

			maSetColor(0xffffff);
			maDrawText(point.x + 55, point.y + 55, touchStr);
		}

		maUpdateScreen();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
