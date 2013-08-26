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

#include "EditBoxScreen.h"
#include "Util.h"
#include <MAUI/EditBox.h>

EditBoxScreen::EditBoxScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "Back");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	Label *label;
	EditBox *editBox;

	label = createLabel("single line", 64);
	editBox = new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2, label, "", 0, gFont, true, false);
	editBox->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = createLabel("multi line input", 128);
	editBox = new EditBox(0, 24, label->getWidth()-PADDING*2, 128-24-PADDING*2, label, "", 0, gFont, true, true, 512);
	editBox->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = createLabel("number input", 64);
	editBox = new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2, label, "", 0, gFont, true, false, 64, EditBox::IM_NUMBERS);
	editBox->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	softKeys = mainLayout->getChildren()[1];

	this->setMain(mainLayout);
}

EditBoxScreen::~EditBoxScreen() {
}

void EditBoxScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		widget->getChildren()[0]->setSelected(true);
	} else {
		widget->getChildren()[0]->setSelected(false);
	}
}

void EditBoxScreen::show() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void EditBoxScreen::hide() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
	Screen::hide();
}

void EditBoxScreen::keyPressEvent(int keyCode, int nativeCode) {
	switch(keyCode) {
			case MAK_UP:
				listBox->selectPreviousItem();
				break;
			case MAK_DOWN:
				listBox->selectNextItem();
				break;
			case MAK_SOFTRIGHT:
				previous->show();
				break;
	}
}

void EditBoxScreen::pointerPressEvent(MAPoint2d point) {
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
			// Do nothing
		}
		else if(softKeys->getChildren()[1]->contains(p)) {
			keyPressEvent(MAK_SOFTRIGHT, 0);
		}
	}
}

void EditBoxScreen::pointerReleaseEvent(MAPoint2d point) {
}
