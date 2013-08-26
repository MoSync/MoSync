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
