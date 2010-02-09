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

#include "MAUIex.h"
#include "LabelScreen.h"
#include "Util.h"

LabelScreen::LabelScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "back");
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
