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

#include "MAUIex.h"
#include "LabelScreen.h"
#include "Util.h"

LabelScreen::LabelScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "Back");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	Label *label;
	label = createLabel("BottomRight", 64);
	label->setHorizontalAlignment(Label::HA_RIGHT);
	label->setVerticalAlignment(Label::VA_BOTTOM);
	label->setSkin(gSkin);
	listBox->add(label);

	label = createLabel
	(
		"Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
		"sed do eiusmod tempor incididunt ut labore et dolore magna "
		"aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
		"ullamco laboris nisi ut aliquip ex ea commodo consequat. "
		"Duis aute irure dolor in reprehenderit in voluptate velit "
		"esse cillum dolore eu fugiat nulla pariatur.",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel("automatic resizing", 32);
	label->setAutoSizeX();
	label->setSkin(gSkin);
	listBox->add(label);

	softKeys = mainLayout->getChildren()[1];

	this->setMain(mainLayout);
}

LabelScreen::~LabelScreen() {
	delete mainLayout;
}

void LabelScreen::keyPressEvent(int keyCode, int nativeCode) {
	switch(keyCode) {
			case MAK_SOFTRIGHT:
				previous->show();
				break;
			case MAK_UP:
				listBox->selectPreviousItem();
				break;
			case MAK_DOWN:
				listBox->selectNextItem();
				break;
	}
}

void LabelScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);
	if(listBox->contains(p)) {
		for(int i = 0; i < listBox->getChildren().size(); i++) {
			if(listBox->getChildren()[i]->contains(p)) {
				listBox->setSelectedIndex(i);
				break;
			}
		}
	}
	else if(softKeys->contains(p)) {
		if(softKeys->getChildren()[0]->contains(p)) {
			keyPressEvent(MAK_SOFTLEFT, 0);
		}
		else if(softKeys->getChildren()[1]->contains(p)) {
			keyPressEvent(MAK_SOFTRIGHT, 0);
		}
	}
}

void LabelScreen::pointerReleaseEvent(MAPoint2d point) {
}
