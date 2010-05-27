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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <ma.h>

#include "MAHeaders.h"

#include <MAUI/Screen.h>
#include <MAUI/Engine.h>

#include <MAUI/Layout.h>
#include <MAUtil/Moblet.h>

using namespace MAUI;
using namespace MAUtil;

class MyMoblet;

#define MAX_IMAGES 100

Widget* createSoftKeyBar(int height, char *left, char *right);

class ObjectWidget : public Widget, MAUtil::TimerListener {
public:
	ObjectWidget(int x, int y, int width, int height, Widget *parent);
	~ObjectWidget();

public:
	void createImage();
	void drawImages();

private:
	MAHandle generateImage(int width, int height, int r, int g, int b);

	void runTimerEvent();
    void drawWidget();

private:
    int numImages;
    MAHandle images[MAX_IMAGES];

};

class MainScreen : public Screen {
public:
	MainScreen(MyMoblet *moblet);
	~MainScreen();
	void keyPressEvent(int keyCode, int nativeCode);
	void pointerPressEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);

private:
	Layout* layout;
	ObjectWidget* objectWidget;
	Widget *softKeys;
	MyMoblet *mMoblet;
};

class MyMoblet : public Moblet  {
public:
	MyMoblet();
	void keyPressEvent(int keyCode, int nativeCode);
	void keyReleaseEvent(int keyCode, int nativeCode);

	void closeEvent();

private:
	Screen *mainScreen;
};

#endif /* _MAIN_H_ */
