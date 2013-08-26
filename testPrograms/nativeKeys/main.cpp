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

#define MIN(a, b) ((a) < (b) ? (a) : (b))

class NKMoblet : public Moblet {
public:
	NKMoblet() {
		printf("Hello World!\n");
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			maExit(0);
		printf("p: 0x%x/0x%x\n", keyCode, nativeCode);
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			maExit(0);
		printf("r: 0x%x/0x%x\n", keyCode, nativeCode);
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new NKMoblet());
	return 0;
}
