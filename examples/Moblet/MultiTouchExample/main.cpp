/* Copyright (C) 2011 Mobile Sorcery AB

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


/**
 * This is an example illustrating how multi touch events works.
 * A map of touchId:s to coordinates are kept in memory. Whenever a touch
 * stops, it is removed from the map.
 *
 * When running in the emulator: right click to simulate multi touches around
 * the center of the screen.
 */

#include <MAUtil/Moblet.h>
#include <MAUtil/Map.h>
#include <mavsprintf.h>

using namespace MAUtil;

class MyMoblet : public Moblet, public IdleListener {
public:
	MyMoblet() {
		Environment::getEnvironment().addIdleListener(this);
	}

	Map<int, MAPoint2d> mTouches;

	/**
	* This function is called with a coordinate when a finger touches the screen.
	*/
	virtual void multitouchPressEvent(MAPoint2d p, int touchId) {
		mTouches[touchId] = p;
	}

	/**
	*  This function is called with a coordinate when a finger has touched the screen and started moving.
	*/
	virtual void multitouchMoveEvent(MAPoint2d p, int touchId) {
		mTouches[touchId] = p;
	}

	/**
	*  This function is called with a coordinate when a finger has stopped touching the screen.
	*/
	virtual void multitouchReleaseEvent(MAPoint2d p, int touchId) {
		mTouches.erase(touchId);
	}

	/**
	 * This function will be called as much as possible, drawing rectangles with the touchId:s
	 * where the fingers touches the screen. The amount of touches possible on a device is platform
	 * dependant.
	 */
	void idle() {
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
