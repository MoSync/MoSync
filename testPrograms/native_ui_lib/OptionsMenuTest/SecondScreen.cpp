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

	Screen::addOptionsMenuItem("Second screen menu item 0", -1, false);
	Screen::addOptionsMenuItem("Second screen menu item 1", -1, false);
	Screen::addOptionsMenuItem("Second screen menu item 2", -1, false);
	Screen::addOptionsMenuItem("Second screen menu item 3", -1, false);
	Screen::addOptionsMenuItem("Second screen menu item 4", -1, false);
	Screen::addOptionsMenuItem("Second screen menu item 5", -1, false);
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
	item->setText("Second Screen event: Menu Item selected: index " + MAUtil::integerToString(index));
	mEventsList->addChild(item);
}
