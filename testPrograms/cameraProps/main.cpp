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
#include <MAUtil/String.h>
#include <conprint.h>
#include <IX_CAMERA.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World\n");
		dumpAll();
	}
	
	void dumpAll() {
		dump("supports.video.capture");
		dump("video.snapshot.encodings");
		dump("camera.orientations");
		dump("camera.resolutions");
	}
	
	void dump(const char* key) {
		int len = maGetSystemProperty(key, NULL, 0);
		printf("%s: %i\n", key, len);
		if(len < 0) {
			return;
		}
		String s(len);
		maGetSystemProperty(key, s.pointer(), len);
		printf("%s\n", s.c_str());
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("%i %i\n", keyCode, nativeCode);
		if(keyCode == MAK_0)
			close();
		if(keyCode == MAK_FIRE)
			dumpAll();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
