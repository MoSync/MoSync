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
#include <conprint.h>

using namespace MAUtil;

class MyMoblet : public Moblet, TimerListener, FocusListener {
private:
	int mStartTime;
public:
	MyMoblet() {
		mStartTime = maGetMilliSecondCount();
		printf("Hello World!\n");
		printf("Press 5 to hide for a few seconds.\n");
		addFocusListener(this);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_5) {
			int res = maSendToBackground();
			printf("stb %i @ %i ms\n", res, maGetMilliSecondCount() - mStartTime);
			addTimer(this, 5000, 1);
		}
		if(keyCode == MAK_0) {
			close();
		}
	}

	void keyReleaseEvent(int keyCode) {
		if(keyCode == MAK_5) {
			printf("r5 @ %i ms\n", maGetMilliSecondCount() - mStartTime);
		}
	}

	void runTimerEvent() {
		int res = maBringToForeground();
		printf("btf %i @ %i ms\n", res, maGetMilliSecondCount() - mStartTime);
	}

	void focusLost() {
		printf("focusLost @ %i ms\n", maGetMilliSecondCount() - mStartTime);
	}
	void focusGained() {
		printf("focusGained @ %i ms\n", maGetMilliSecondCount() - mStartTime);
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
};
