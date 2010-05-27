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

#include <maapi.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include <maassert.h>
#include <mastdlib.h>
#include <MAP/MemoryMgr.h>

#include "main.h"
#include "util.h"

using namespace MAUtil;


MainScreen::MainScreen(MyMoblet *moblet)
	: mHttp(this), mMoblet(moblet) {
	layout = new Layout(0, 0, gScreenWidth, gScreenHeight, NULL, 1, 2);

	softKeys = createSoftKeyBar(30, "", "Exit");

	listBox = new ListBox(0, 0, gScreenWidth, gScreenHeight-softKeys->getHeight(), layout, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true);
	listBox->setPaddingLeft(PADDING);
	listBox->setPaddingRight(PADDING);
	listBox->setPaddingTop(PADDING);
	listBox->setPaddingBottom(PADDING);

	layout->add(softKeys);

	int res = mHttp.create("http://www.example.com/", HTTP_GET);
	if(res < 0) {
		char tbuf[256];
		sprintf(tbuf, "http.create fail %i", res);
		addListBoxItem(tbuf);
	} else {
		mHttp.finish();
	}

	this->setMain(layout);
}

MainScreen::~MainScreen() {
	delete layout;
}

void MainScreen::addListBoxItem(String s) {
	Label *label = createLabel(s.c_str());
	listBox->add(label);
}

void MainScreen::httpFinished(HttpConnection* http, int result) {
	char tbuf[256];

	sprintf(tbuf, "HTTP %i", result);
	addListBoxItem(tbuf);

	if(result <= 0)
		return;
	String cl;
	int res = mHttp.getResponseHeader("content-length", &cl);
	sprintf(tbuf, "cl code %i", res);
	addListBoxItem(tbuf);
	if(res > 0) {
		sprintf(tbuf, "cl strlen %i", cl.length());
		addListBoxItem(tbuf);
		sprintf(tbuf, "cl: %s", cl.c_str());
		addListBoxItem(tbuf);
		int len = atoi(cl.c_str());
		if(len >= BUFSIZE) {
			sprintf(tbuf, "Buffer too small, can't read.");
			addListBoxItem(tbuf);
			return;
		}
		mBuffer[len] = 0;
		mHttp.read(mBuffer, len);
	}
}

void MainScreen::connReadFinished(Connection* conn, int result) {
	char tbuf[256];
	sprintf(tbuf, "connReadFinished %i", result);
	addListBoxItem(tbuf);
}

void MainScreen::keyPressEvent(int keyCode, int nativeCode) {
	switch(keyCode) {
	case MAK_UP:
		listBox->selectPreviousItem();
		break;
	case MAK_DOWN:
		listBox->selectNextItem();
		break;
	case MAK_SOFTRIGHT:
		mMoblet->closeEvent();
		mMoblet->close();
		break;
	}
}

void MainScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);
	if(listBox->contains(p)) {
		for(int i = 0; i < listBox->getChildren().size(); i++) {
			if(listBox->getChildren()[i]->contains(p)) {
				listBox->setSelectedIndex(i);
				break;
			}
		}
	} else if(softKeys->contains(p)) {
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
	gSkin = new WidgetSkin(RES_SELECTED, RES_UNSELECTED, 3, 18, 10, 11, true, true);
	Engine& engine = Engine::getSingleton();
	engine.setDefaultFont(gFont);
	engine.setDefaultSkin(gSkin);

	MAExtent screenSize = maGetScrSize();
	gScreenWidth = EXTENT_X(screenSize);
	gScreenHeight = EXTENT_Y(screenSize);

	mainScreen = new MainScreen(this);
	mainScreen->show();
}

extern "C" int MAMain() {
	Moblet::run( newobject( MyMoblet, new MyMoblet( ) ) );
	return 0;
}
