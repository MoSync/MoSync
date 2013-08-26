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
#include <MAUI/Screen.h>
#include <MAUI/EditBox.h>

using namespace MAUtil;
using namespace MAUI;

class MyScreen : public Screen {
public:
	MyScreen() {
		//todo: get a maui font, or this editbox will not render.
		EditBox* eb = new EditBox(0, 0, 200, 30, NULL);
		setMain(eb);
		eb->setInputMode(EditBox::IM_QWERTY);
		eb->activate();
	}
	
	~MyScreen() {
		// todo: delete main widget of this screen 
	}
private:
};

class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {
		// todo: initializtion
		screen = new MyScreen();
		screen->show();
	}

	void keyPressEvent(int keyCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}
	MyScreen* screen;

	~MAUIMoblet() {
		delete screen;
	}
	
};

extern "C" int MAMain() {
	Moblet::run(new MAUIMoblet());
	return 0;
};
