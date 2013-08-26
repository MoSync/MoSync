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

#include "LayoutScreen.h"
#include "MAHeaders.h"
#include "Util.h"
#include <MAUI/ListBox.h>
#include <MAUI/Image.h>

LayoutScreen::LayoutScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "Back");
	this->setMain(mainLayout);
	ListBox* listBox = (ListBox*) mainLayout->getChildren()[0];
	Layout *layout = new Layout(0, 0, scrWidth, scrHeight-mainLayout->getChildren()[1]->getHeight(), listBox, 3, 4);
	layout->setMarginX(5);
	layout->setMarginY(5);
	layout->setPaddingLeft(5);
	layout->setPaddingRight(5);
	layout->setDrawBackground(false);

	for(int i = RES_ICONS_START; i != RES_ICONS_END+1; i++) {
		new Image(0, 0, 0, 0, layout, true, true, i);
	}

	softKeys = mainLayout->getChildren()[1];
}

LayoutScreen::~LayoutScreen() {}

void LayoutScreen::keyPressEvent(int keyCode, int nativeCode) {
	if(keyCode == MAK_SOFTRIGHT) {
		previous->show();
	}
}

void LayoutScreen::pointerPressEvent(MAPoint2d point) {
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

void LayoutScreen::pointerReleaseEvent(MAPoint2d point) {
}
