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
#include <MAUI/Clock.h>
#include <MAUI/Engine.h>
#include <conprint.h>

using namespace MAUtil;
using namespace MAUI;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//first MAUI example
class First : public Moblet {
public:
	First() {
		printf("Hello World!\n");
		Extent s = maGetScrSize();

		//Create a widget. Clock, I choose you!
		mWidget = new Clock(0, 0, EXTENT_X(s), EXTENT_Y(s), NULL, true);

		//Widgets are organized in a logical tree.
		//To make the this widget visible, we must set it to be the root of that tree.
		Engine::getSingleton().setMain(mWidget);

		//Now we can just let the system handle the rest.
	}

	~First() {
		delete mWidget;
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	Widget* mWidget;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new First());
}
