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

//Font, Image, Label, Layout, Listbox, Screen, WidgetSkin

#include <MAUtil/Moblet.h>

#include <MAUI/Engine.h>
#include <MAUI/Font.h>
#include <MAUI/Image.h>
#include <MAUI/Label.h>
#include <MAUI/Layout.h>
#include <MAUI/Listbox.h>
#include <MAUI/Screen.h>
#include <MAUI/WidgetSkin.h>

#include "MAHeaders.h"

using namespace MAUtil;
using namespace MAUI;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static const char* sLabelTexts[] = { "Hello World!", "MAUI Multi-example", "Font: Comic Sans",
"line3", "4", "5", "6", "7", "8", "9", "10" };
static const int snLabelTexts = sizeof(sLabelTexts) / sizeof(char*);

//MAUI example that shows multiple features
class Multi : public Moblet {
public:
	Multi() {
		//initialize font
		mFont = new Font(RES_FONT);

		//create first screen

		//a vertical listbox, with wrapping but without size.
		mListBox = new ListBox(0, 0, 0, 0, NULL, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true);
		//default black background
		mListBox->setDrawBackground();
		for(int i=0; i<snLabelTexts; i++) {
			//a label with colored background, a text and a font, but no size.
			//specifying a ListBox as a parent adds the new Widget to the end of the list
			Label* label = new Label(0, 0, 200, 32, mListBox, sLabelTexts[i], 0x80 << (i*8), mFont);
			label->setDrawBackground();
			//make the label autosize itself
			label->setAutoSizeX();
			label->setAutoSizeY();
		}
		//a Screen
		mScr1 = new Screen;
		//this sets the size of our listbox
		mScr1->setMain(mListBox);
		//activate the screen
		mScr1->show();


		//construct a second screen

		//with a 2-by-3 grid layout
		mLayout = new Layout(0, 0, 0, 0, NULL, 2, 3);
		//that has automatic size
		mLayout->setAutoSizeX();
		mLayout->setAutoSizeY();
		//and a skin
		mSkin = new WidgetSkin(RES_SKIN_BLUE, RES_SKIN_BLACK, 32, 64, 32, 64);
		mLayout->setSkin(mSkin);
		mLayout->setDrawBackground();	//enables drawing of skin

		//when Widgets are constructed with a Layout parent, they are added to the layout,
		//in left-to-right, top-to-bottom order.

		new Image(16, 16, 0, 0, mLayout, RES_IMAGE);
		new Label(0, 0, 0, 0, mLayout, "meh", 0, mFont);
		new Label(0, 0, 0, 0, mLayout, "bleem!", 0, mFont);
		new Image(16, 16, 0, 0, mLayout, RES_IMAGE);

		mScr2 = new Screen;
		mScr2->setMain(mLayout);
		//the second screen is not activated here. see keyPressEvent().
	}

	~Multi() {
		delete mScr1;
		delete mListBox;
		//Widget's destructor deletes all of its children,
		//so we don't delete our Labels and Images here.

		delete mLayout;
		delete mSkin;
		delete mScr2;

		delete mFont;
	}

	void keyPressEvent(int keyCode) {
		switch(keyCode) {
		case MAK_0:
			maExit(0);
		case MAK_RIGHT:
			mScr2->show();
			break;
		case MAK_LEFT:
			mScr1->show();
			break;
		case MAK_DOWN:
			mListBox->selectNextItem();
			break;
		case MAK_UP:
			mListBox->selectPreviousItem();
			break;
		}
	}

private:
	Font* mFont;

	ListBox* mListBox;
	Screen* mScr1;

	Layout* mLayout;
	WidgetSkin* mSkin;
	Screen* mScr2;
};

extern "C" int MAMain() {
	Moblet::run(new Multi());
}
