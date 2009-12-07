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
#include "Engine.h"
//#include <conprint.h>
#include <stdio.h>

namespace MAUI {

	Screen* Screen::currentScreen = NULL;
	
	Screen::Screen() : main(0) {
		hide();
	}
	
	void Screen::show() {
		//printf("showing screen!\n");
		
		if(!main) {
			return;
		}

		if(currentScreen) currentScreen->hide();
		currentScreen = this;
		Engine::getSingleton().setMain(main);
		main->setEnabled(true);
		main->setDirty(true);
		//printf("Requesting UI update...!\n");
		Engine::getSingleton().requestUIUpdate();
		Environment::getEnvironment().addKeyListener(this);
		Environment::getEnvironment().addPointerListener(this);
	}
	
	
	void Screen::setMain(Widget* main) {
		MAExtent scrSize = maGetScrSize();
		this->main = main;
		if(!main) return;
		main->setPosition(0,0);
		main->setWidth(EXTENT_X(scrSize));
		main->setHeight(EXTENT_Y(scrSize));
		Environment& env = Environment::getEnvironment();
		if(!env.isKeyListener(this))
			main->setEnabled(false);
	}


	Widget* Screen::getMain() {
		return main;
	}

	void Screen::hide() {
		//printf("hiding screen!\n");
		Environment& env = Environment::getEnvironment();
		if(env.isKeyListener(this))
			env.removeKeyListener(this);
		if(env.isPointerListener(this))
			env.removePointerListener(this);
		if(main)
			main->setEnabled(false);
	}

	Screen::~Screen() {
		/*if(this == currentScreen)
			Environment::getEnvironment().removeKeyListener(this); */
	}
}
