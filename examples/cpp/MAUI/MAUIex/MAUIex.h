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

#ifndef _MAUIEX_H_
#define _MAUIEX_H_

#include <ma.h>

#include "MAHeaders.h"

#include <MAUI/Screen.h>
#include <MAUI/Engine.h>
#include <MAUI/Font.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>
#include <MAUI/Image.h>
#include <MAUI/Layout.h>
#include <MAUI/EditBox.h>
#include <MAUtil/Moblet.h>

using namespace MAUI;
using namespace MAUtil;

Widget* createSoftKeyBar(int height, char *left, char *right);

class MainScreen : public Screen {
public:
	MainScreen();
	~MainScreen();
	void keyPressEvent(int keyCode, int nativeCode);
	void pointerPressEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
private:
	Vector<Screen*> screens;
	ListBox* listBox;
	Layout* layout;
	Widget *softKeys;
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

#endif /* _MAUIEX_H_ */
