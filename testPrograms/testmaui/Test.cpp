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

#include <MAAPI.h>
#include <MAUI/Engine.h>
#include <MAUI/Font.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>
#include <MAUI/Image.h>
#include <MAUI/Layout.h>
#include <MAUI/ImageGenerators.h>
#include "MAHeaders.h"

using namespace MAUI;

int navigatorIndex = 0;
ListBox *listBox;
ListBox *listBox2[8];

class ListBoxIdleListener : public IdleListener {
public:
	ListBoxIdleListener(ListBox* navigator) {
		this->navigator = navigator;
		startTime = maGetMilliSecondCount();
	}

	void idle() {
		if(maGetMilliSecondCount()-startTime < delay) {
			return;
		}

		startTime = maGetMilliSecondCount();

		if(key == MAK_UP || key == MAK_2) {
			listBox->setPrev();
		} else 
		if(key == MAK_DOWN || key == MAK_8 ) {
			listBox->setNext();
		}

		if(delay > 50)delay -= 50;
	}

	ListBox* navigator;
	int delay;
	int startTime;
	int key;
	
};



class ListBoxListener : public MAUI::EventListener {
public:
	ListBoxListener(ListBoxIdleListener *idleListener) : idleListener(idleListener) {}

	void event(EVENT& e) {
		if(e.key == MAK_UP || e.key == MAK_DOWN || e.key == MAK_2 || e.key == MAK_8) {
			if(e.type == EVENT_TYPE_KEY_PRESSED) {
				Engine::getSingleton().registerIdleListener(idleListener);
				ListBox *navigator = idleListener->navigator;
	
				idleListener->startTime = maGetMilliSecondCount();
				if(e.key == MAK_UP || e.key == MAK_2) {
					listBox->setPrev();
				} else 
				if(e.key == MAK_DOWN || e.key == MAK_8) {
					listBox->setNext();
				}
			} else 	if(e.type == EVENT_TYPE_KEY_RELEASED) {		
				Engine::getSingleton().unregisterIdleListener(idleListener);
			}

			idleListener->delay = 300;
			idleListener->key   = e.key;
		}

	}

private:
	ListBoxIdleListener* idleListener;
};

class NavigatorListener : public MAUI::EventListener {
public:
	NavigatorListener() {}

	void event(EVENT& e) {
		if(e.type == EVENT_TYPE_KEY_PRESSED && (e.key == MAK_LEFT || e.key == MAK_4)) {
			listBox2[listBox->getSelectedIndex()]->setPrev();
		} else 
		if(e.type == EVENT_TYPE_KEY_PRESSED && (e.key == MAK_RIGHT || e.key == MAK_6)) {
			listBox2[listBox->getSelectedIndex()]->setNext();
		}
	}

private:
};


void addNewsItem(ListBox *listBox, Handle pic, char *text) {
	Extent screenSize = maGetScrSize();
	Extent imgSize = maGetImageSize(pic);
	int scrWidth = EXTENT_X(screenSize);
	int scrHeight = EXTENT_Y(screenSize);
	Image *image;
	Label *label;
	image = new Image(0, 0, 0, 0, listBox, BACK);		
	Layout *layout = new Layout(0, 0, scrWidth, EXTENT_Y(imgSize), image, Layout::LT_HORIZONTAL_STACKING);
	image = new Image(0, 0, 0, 0, layout, pic, false);	
	label = new Label(4, 4, scrWidth-EXTENT_X(imgSize), EXTENT_Y(imgSize), layout, text, 0, true);	
	label->setVerticalPlacement(Label::VP_TOP); label->setHorizontalPlacement(Label::HP_LEFT);
	label->setVerticalAlignment(4); 		label->setHorizontalAlignment(4);
}

extern "C" int MAMain() {
	Font *font = new MAUI::Font(FONT, FONT_METRICS, 0xffff00);

	Engine& engine = Engine::getSingleton();
	engine.setSystemFont(font);

	Extent screenSize = maGetScrSize();
	int scrWidth = EXTENT_X(screenSize);
	int scrHeight = EXTENT_Y(screenSize);

	ListBox *navigator = new ListBox(0, 0, scrWidth, scrHeight, NULL, 0, LBO_HORIZONTAL, LBWM_ANIMATED);

	Handle gradient = BACK;
	Handle gradient2 = BACK2;
	Extent imgSize1 = maGetImageSize(gradient);
	Extent imgSize2 = maGetImageSize(gradient2);
	int imgWidth1 = EXTENT_X(imgSize1);
	int imgWidth2 = EXTENT_X(imgSize2);
	int imgHeight1 = EXTENT_Y(imgSize1);
	int imgHeight2 = EXTENT_Y(imgSize2);

	Extent logoSize = maGetImageSize(LOGO);
	int logoWidth = EXTENT_X(logoSize);
	int logoHeight = EXTENT_Y(logoSize);

/*
	Handle gradient = maCreatePlaceholder();
	maCreateDrawableImage(gradient, 240, 50);
	ImageGenerators::linearGradient(gradient, Point(80, 0), Point(80, 170), 0xff2264aa, 0xff000000);

	Handle gradient2 = maCreatePlaceholder();
	maCreateDrawableImage(gradient2, 240, 100);
	ImageGenerators::circularGradient(gradient2, Point(120, 50), 200, 0xffaa1111, 0xff000000);
*/
	/*
	Image *image;
	Label *label;
	char temp[64];
	for(int i = 0; i < 3; i++) {
		listBox =  new ListBox(0, 0, scrWidth, scrHeight, navigator, 0, LBO_VERTICAL);
		for(int j = 0; j < 20; j++) {
			sprintf(temp, "listbox element %d", j+i*20);
			int randVal = rand()%2;
			if(randVal == 0) {
				image = new Image(0, 0, imgWidth1, imgHeight1, listBox, gradient);
				label = new Label(0, 0, imgWidth1, imgHeight1, image, temp, 0x3f7533, true);	
			}
			else {
				image = new Image(0, 0, imgWidth2, imgHeight2, listBox, gradient2);
				label = new Label(0, 0, imgWidth2, imgHeight2, image, temp, 0x3f7533, true);			
			}
			label->setHorizontalPlacement(Label::HP_LEFT);
		}
	}*/

	Layout *mainLayout = new Layout(0, 0, scrWidth, scrHeight, NULL, Layout::LT_VERTICAL_STACKING); 
	Image *logoImage = new Image(0, 0, 0, 0, mainLayout, LOGO, true);
	Image  *backGround = new Image(0, 0, 0, 0, mainLayout, BACK2, false);
	listBox =  new ListBox(0, 0, scrWidth, scrHeight-logoHeight, backGround, 0, LBO_VERTICAL);

	for(int i = 0; i < 8; i++) {
		Extent imgSize = maGetImageSize(IMG1);
		listBox2[i] =  new ListBox(0, 0, scrWidth, EXTENT_Y(imgSize), listBox, 0, LBO_HORIZONTAL);
		for(int j = 0; j < 4; j++) {
			char *text;
			int textInd = rand()%4;
			switch(textInd) {
				case 0: text = "Will the mobile market explode?"; break;
				case 1: text = "Iphone unleashed, review inside."; break;
				case 2: text = "Fire at will! said Lt Daniels."; break;
				case 3: text = "BitTorrent becomes closed source."; break;
			}
			addNewsItem(listBox2[i], IMG1 + textInd, text);
		}

	}

	//Engine *engine = new Engine(label, font);	
	ListBoxIdleListener *idleListener = new ListBoxIdleListener(navigator);
	engine.registerEventListner(new ListBoxListener(idleListener));
	engine.registerEventListner(new NavigatorListener());	
	engine.setMain(mainLayout);

	engine.run();
	
	return 0;
}
