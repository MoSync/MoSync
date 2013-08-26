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

#include "CustomScreen.h"
#include "Util.h"

CustomScreen::CustomScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "Back");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	digClock = new Clock(0, 0, scrWidth-PADDING*2, 32, listBox, gFont);
	digClock->setType(Clock::DIGITAL);

	anaClock = new Clock(0, 0, scrWidth-PADDING*2, 128-32, listBox, 0);
	anaClock->setType(Clock::ANALOGUE);

	softKeys = mainLayout->getChildren()[1];

	setMain(mainLayout);
}

CustomScreen::~CustomScreen() {
}

void CustomScreen::keyPressEvent(int keyCode, int nativeCode) {
	if(keyCode == MAK_SOFTRIGHT) {
		previous->show();
	}
}

void CustomScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);
	if(softKeys->contains(p)) {
		if(softKeys->getChildren()[0]->contains(p)) {
			// Do nothing
		}
		else if(softKeys->getChildren()[1]->contains(p)) {
			keyPressEvent(MAK_SOFTRIGHT, 0);
		}
	}
}

void CustomScreen::pointerReleaseEvent(MAPoint2d point) {
}
