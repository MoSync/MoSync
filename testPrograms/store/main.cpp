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

#define BUFSIZE 1024

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		Handle store = maOpenStore("store.bin", MAS_CREATE_IF_NECESSARY);
		printf("open: %i\n", store);
		if(store < 0)
			return;
		Handle data = maCreatePlaceholder();
		printf("phc: 0x%x", data);
		maCreateData(data, BUFSIZE);
		char buf[BUFSIZE];
		maWriteData(data, buf, 0, BUFSIZE);
		int res = maWriteStore(store, data);
		printf("write: %i\n", res);
		maCloseStore(store, 0);
		printf("done.\n");
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			close();
		}
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
