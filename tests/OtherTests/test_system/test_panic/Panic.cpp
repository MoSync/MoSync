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

/** \file Panic.cpp
* TEST PANIC
*
* TEST PANIC causes a MoSync Panic by trying to write to an invalid data object.
* Only for internal test purposes.
*
* \author Ann-Sofie Lindblom
*/
#include "MAHeaders.h"
#include <MAUtil/Moblet.h>
#include <conprint.h>
#include "../examples/common/helpers.h"
#include <maassert.h>

using namespace MAUtil;

class PanicMoblet : public Moblet {
public:
	PanicMoblet() {
		// Open and read storage
		printf("Don't panic!\n");
		/*Handle test_panic = maOpenStore("TestPanic.sav", MAS_CREATE_IF_NECESSARY);
		maReadStore(test_panic, RES_STORE);
		maCloseStore(test_panic, true);*/

		// Destroy data object and then try to write to it
		//maDestroyObject(TMP_STORE);
		//maCreateData(TMP_STORE, 0);
		int tmp = 17;
		//if(maGetDataSize(TMP_STORE) <= 0) {
			//PANIC_MESSAGE("Data object empty.");
			//FREEZE;
		//}
		maReadData(TMP_STORE, &tmp, 0, sizeof(int));
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

	void keyReleaseEvent(int keyCode) {}

private:
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new PanicMoblet());
	return 0;
};
