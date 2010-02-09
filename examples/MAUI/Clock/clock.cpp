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

#include <MAUtil/Moblet.h>
#include <MAUI/Clock.h>
#include <MAUI/Engine.h>
#include <conprint.h>

using namespace MAUtil;
using namespace MAUI;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//first MAUI example
class First : public Moblet {
public:
	First() {
		printf("Hello World!\n");
		Extent s = maGetScrSize();

		//Create a widget. Clock, I choose you!
		mWidget = new Clock(0, 0, EXTENT_X(s), EXTENT_Y(s), NULL, true);

		//Widgets are organized in a logical tree.
		//To make the this widget visible, we must set it to be the root of that tree.
		Engine::getSingleton().setMain(mWidget);

		//Now we can just let the system handle the rest.
	}

	~First() {
		delete mWidget;
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	Widget* mWidget;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new First());
}
