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

#include "Screen.h"
#include <MAUI/Engine.h>
//#include <conprint.h>
#include <stdio.h>

namespace MAUI {

	Screen* Screen::sCurrentScreen = NULL;
	
	Screen::Screen() : mMain(0) {
		hide();
	}
	
	void Screen::show() {
		lprintfln("showing screen!\n");
		
		if(!mMain) {
			return;
		}

		if(sCurrentScreen) sCurrentScreen->hide();
		sCurrentScreen = this;
		Engine::getSingleton().setMain(mMain);
		mMain->setEnabled(true);
		mMain->setDirty(true);
		//printf("Requesting UI update...!\n");
		Engine::getSingleton().requestUIUpdate();

		lprintfln("adding self to environment!\n");
		Environment::getEnvironment().addKeyListener(this);
		Environment::getEnvironment().addPointerListener(this);
	}
	
	
	void Screen::setMain(Widget* main) {
		lprintfln("Screen::setMain(%x)", main);
		MAExtent scrSize = maGetScrSize();
		this->mMain = main;
		if(!mMain) return;
		mMain->setPosition(0,0);
		mMain->setWidth(EXTENT_X(scrSize));
		mMain->setHeight(EXTENT_Y(scrSize));
		Environment& env = Environment::getEnvironment();
		if(!env.isKeyListener(this)) {
			mMain->setEnabled(false);
		}
	}


	Widget* Screen::getMain() {
		return mMain;
	}

	void Screen::hide() {
		//printf("hiding screen!\n");
		Environment& env = Environment::getEnvironment();
		if(env.isKeyListener(this))
			env.removeKeyListener(this);
		if(env.isPointerListener(this))
			env.removePointerListener(this);
		if(mMain)
			mMain->setEnabled(false);
	}

	void Screen::keyPressEvent(int keyCode) {
		lprintfln("Screen::keyPressed()");
		mMain->keyPressed(keyCode);
	}

	void Screen::keyReleaseEvent(int keyCode) {
		lprintfln("Screen::keyReleaseEvent() 1");
		lprintfln("mMain: %x", mMain);
		mMain->keyReleased(keyCode);
		lprintfln("Screen::keyReleaseEvent() 2");
	}

	void Screen::pointerPressEvent(MAPoint2d point) {
		mMain->pointerPressed(point);
	}

	void Screen::pointerReleaseEvent(MAPoint2d point) {
		mMain->pointerReleased(point);
	}

	void Screen::pointerMoveEvent(MAPoint2d point) {
		mMain->pointerMoved(point);
	}


	Screen::~Screen() {
		/*if(this == currentScreen)
			Environment::getEnvironment().removeKeyListener(this); */
	}
}
