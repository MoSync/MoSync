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

#ifndef COMMON_H
#define COMMON_H

#include <MATest/Test.h>
#include <MAUtil/Environment.h>
#include <MAUtil/CharInput.h>

using namespace MATest;
using namespace MAUtil;

#define TK_YES MAK_SOFTLEFT
#define TK_NO MAK_SOFTRIGHT
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define BLACK 0
#define GREEN 0x00FF00
#define RED 0xFF0000
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define DATA_SIZE (4*1024)

/*
int waitTest() {
	MAEvent event;
	while(maGetEvent(&event)) {
		switch(event.type) {
			case EVENT_TYPE_KEY_RELEASED:
				if(event.key == TK_YES) return TK_YES;
				else return TK_NO;

			case EVENT_TYPE_POINTER_RELEASED:
				return (((EXTENT_X(maGetScrSize())/ 2) - event.point.x) < 0) ? TK_YES : TK_NO;
			case EVENT_TYPE_CLOSE: maExit(1);
		}
	}
}
*/

class KeyBaseCase : public TestCase, public KeyListener, public PointerListener {
public:
	KeyBaseCase(const String& name) : TestCase(name) {}

	//TestCase
	virtual void open() {
		Environment::getEnvironment().addKeyListener(this);
		Environment::getEnvironment().addPointerListener(this);

	}
	virtual void close() {
		Environment::getEnvironment().removeKeyListener(this);
		Environment::getEnvironment().removePointerListener(this);
	}

	void pointerPressEvent(MAPoint2d p) {
	}

	void pointerMoveEvent(MAPoint2d p) {
	}

	void pointerReleaseEvent(MAPoint2d p) {
		int res = ((p.x - (EXTENT_X(maGetScrSize())/ 2)) < 0) ? TK_YES : TK_NO;
		assert(name, res == TK_YES);
		suite->runNextCase();
	}


	void checkYesNo(int keyCode);
};

template<void func()> class TemplateCase : public KeyBaseCase {
public:
	TemplateCase(const String& name) : KeyBaseCase(name) {}

	//TestCase
	virtual void start() {
		func();
	}

	//KeyListener
	virtual void keyPressEvent(int keyCode) {
	}

	virtual void keyReleaseEvent(int keyCode) {
		checkYesNo(keyCode);
	}
};

class CharInputBaseCase : public TestCase, public CharInputListener {
public:
	CharInputBaseCase(const String& name) : TestCase(name) {}

	//TestCase
	virtual void open() {
		MAUtil::CharInput::getCharInput().addCharInputListener(this);
	}
	virtual void close() {
		MAUtil::CharInput::getCharInput().removeCharInputListener(this);
	}
};

template<void func()> class TemplateCharInputCase : public CharInputBaseCase {
public:
	TemplateCharInputCase(const String& name) : CharInputBaseCase(name) {}

	//TestCase
	virtual void start() {
		func();
	}

	// CharInputListener
	virtual void characterChanged(char c)
	{
	}

	virtual void characterDeployed(char c)
	{
	}
};


struct Dimensions {
	unsigned short width, height;
};

void clearScreen(int color=BLACK);

#endif	//COMMON_H
