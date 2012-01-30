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
