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
#include <ma.h>

using namespace MAUtil;

class MyMoblet : public Moblet, TimerListener {
public:
	MyMoblet() {
		printf("Hello World!\n");
		addTimer(this, 3000, 0);
	}

	void customEvent(const MAEvent& e) {
		printf("cE: %i\n", e.type);
	}

	void runTimerEvent() {
		printf("timer'd!!\n");
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("kP: %i, %i\n", keyCode, nativeCode);
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
