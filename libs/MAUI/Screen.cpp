/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Screen.h"
#include "Engine.h"
//#include <conprint.h>

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
