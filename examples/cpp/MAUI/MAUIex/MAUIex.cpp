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

#include "MAUIex.h"

#include "LabelScreen.h"
#include "ImageScreen.h"
#include "LayoutScreen.h"
#include "EditBoxScreen.h"
#include "CustomScreen.h"

#include "Util.h"

#include <conprint.h>

MyMoblet *moblet;

MainScreen::MainScreen() {
	screens.add(new LabelScreen(this));
	screens.add(new ImageScreen(this));
	screens.add(new EditBoxScreen(this));
	screens.add(new LayoutScreen(this));
	screens.add(new CustomScreen(this));

	layout = createMainLayout("Select", "Exit");
	listBox = (ListBox*) layout->getChildren()[0];

	listBox->add(createLabel("Label / ListBox"));
	listBox->add(createLabel("Image"));
	listBox->add(createLabel("EditBox"));
	listBox->add(createLabel("Layout"));
	listBox->add(createLabel("Custom"));

	softKeys = layout->getChildren()[1];

	this->setMain(layout);
}

MainScreen::~MainScreen() {
	delete layout;
	for(int i = 0; i < screens.size(); i++) delete screens[i];
}

void MainScreen::keyPressEvent(int keyCode, int nativeCode) {
	switch(keyCode) {
	case MAK_UP:
		listBox->selectPreviousItem();
		break;
	case MAK_DOWN:
		listBox->selectNextItem();
		break;
	case MAK_FIRE:
	case MAK_SOFTLEFT:
		{
			int index = listBox->getSelectedIndex();
			if(index == screens.size()+1) {
				moblet->closeEvent();
				moblet->close();
			}
			else
				screens[index]->show();
		}
		break;
	case MAK_SOFTRIGHT:
		moblet->closeEvent();
		moblet->close();
		break;
	}
}

void MainScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);
	if(listBox->contains(p)) {
		for(int i = 0; i < listBox->getChildren().size(); i++) {
			if(listBox->getChildren()[i]->contains(p)) {
				int index = listBox->getSelectedIndex();
				if(index == i) {
					keyPressEvent(MAK_FIRE, 0);
				}
				else {
					listBox->setSelectedIndex(i);
				}
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

void MainScreen::pointerReleaseEvent(MAPoint2d point) {
}

void MyMoblet::keyPressEvent(int keyCode, int nativeCode) {
}

void MyMoblet::keyReleaseEvent(int keyCode, int nativeCode) {
}

void MyMoblet::closeEvent() {
	// do destruction here
	delete mainScreen;
}

MyMoblet::MyMoblet() {
	gFont = new MAUI::Font(RES_FONT);
	gSkin = new WidgetSkin(RES_SELECTED, RES_UNSELECTED, 16, 32, 16, 32, true, true);
	Engine& engine = Engine::getSingleton();
	engine.setDefaultFont(gFont);
	engine.setDefaultSkin(gSkin);

	MAExtent screenSize = maGetScrSize();
	scrWidth = EXTENT_X(screenSize);
	scrHeight = EXTENT_Y(screenSize);
	mainScreen = new MainScreen();
	mainScreen->show();

}

extern "C" int MAMain() {
	moblet = new MyMoblet();
	MyMoblet::run(moblet);
	return 0;
}
