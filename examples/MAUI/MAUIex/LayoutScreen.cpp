/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
