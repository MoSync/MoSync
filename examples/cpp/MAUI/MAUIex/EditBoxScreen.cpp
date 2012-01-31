/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
