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

class MyMoblet : public Moblet, public TimerListener {
public:

	MyMoblet() {
		printf("Hello World!\n");
		addTimer(this, 20, 0);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("Button %d pressed.  Native %d",keyCode,nativeCode);
		if (keyCode == MAK_0){
			maExit(0);
		}
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}

	void runTimerEvent() {
		maResetBacklight();	//does not help
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
