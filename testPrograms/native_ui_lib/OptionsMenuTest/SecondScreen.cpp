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

/**
 * @file SecondScreen.cpp
 * @author emma
 */

#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "SecondScreen.h"

/**
 * Constructor.
 */
SecondScreen::SecondScreen() :
		Screen(), mMainLayout(NULL), mEventsList(NULL)

{
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x104E8B);
	Screen::setMainWidget(mMainLayout);
	this->setTitle("Second");

	Label* info = new Label();
	info->setText("Second screen.6 Menu items have no icons.");
	mMainLayout->addChild(info);

	mEventsList = new ListView();
	mEventsList->fillSpaceHorizontally();
	mEventsList->fillSpaceVertically();
	mMainLayout->addChild(mEventsList);

	Screen::addOptionsMenuItem("Second screen menu item 0");
	Screen::addOptionsMenuItem("Second screen menu item 1");
	Screen::addOptionsMenuItem("Second screen menu item 2");
	Screen::addOptionsMenuItem("Second screen menu item 3");
	Screen::addOptionsMenuItem("Second screen menu item 4");
	Screen::addOptionsMenuItem("Second screen menu item 5");
	this->addScreenListener(this);
}

/**
 * Destructor.
 */
SecondScreen::~SecondScreen() {
	this->removeScreenListener(this);
}

/**
 * This method is called when the OptionsMenu is being closed
 * (either by the user canceling the menu with the back/menu
 * button, or when an item is selected.
 * @param Screen The screen that generated the event.
 */
void SecondScreen::optionsMenuClosed(Screen* screen) {

	ListViewItem* item = new ListViewItem();
	item->setText("Second Screen event: Menu closed.");
	mEventsList->addChild(item);
}

/**
 * This method is called when an options menu item is selected.
 * @param index The index on which the item is placed into the
 * OptionsMenu.
 *  @param Screen The screen that generated the event.
 */
void SecondScreen::optionsMenuItemSelected(Screen* screen, int index) {
	ListViewItem* item = new ListViewItem();
	item->setText("Second Screen event: Menu Item selected: index " +
			MAUtil::integerToString(index));
	mEventsList->addChild(item);
}
