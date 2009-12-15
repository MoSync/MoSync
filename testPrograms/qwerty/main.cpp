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
#include <MAUI/Screen.h>
#include <MAUI/EditBox.h>

using namespace MAUtil;
using namespace MAUI;

class MyScreen : public Screen {
public:
	MyScreen() {
		//todo: get a maui font, or this editbox will not render.
		EditBox* eb = new EditBox(0, 0, 200, 30, NULL);
		setMain(eb);
		eb->setQwerty();
		eb->activate();
	}
	
	~MyScreen() {
		// todo: delete main widget of this screen 
	}
private:
};

class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {
		// todo: initializtion
		screen = new MyScreen();
		screen->show();
	}

	void keyPressEvent(int keyCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}
	MyScreen* screen;

	~MAUIMoblet() {
		delete screen;
	}
	
};

extern "C" int MAMain() {
	Moblet::run(new MAUIMoblet());
	return 0;
};
