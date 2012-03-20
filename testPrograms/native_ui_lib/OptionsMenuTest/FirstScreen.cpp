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
 * @file FirstScreen.cpp
 * @author emma
 */

#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "FirstScreen.h"

#define CLEAR_MENU "Clear Menu"
#define RECREATE_MENU "Recreate Menu"

/**
 * Constructor.
 */
FirstScreen::FirstScreen() :
		Screen(), mMainLayout(NULL), mEventsList(NULL)

{
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x104E8B);
	Screen::setMainWidget(mMainLayout);
	this->setTitle("First");

	Label* info = new Label();
	info->setText("First screen.2 Menu items with icons:save and search");
	mMainLayout->addChild(info);

	mClearOptions = new Button();
	mClearOptions->setText(CLEAR_MENU);
	mMainLayout->addChild(mClearOptions);

	mEventsList = new ListView();
	mEventsList->fillSpaceHorizontally();
	mEventsList->fillSpaceVertically();
	mMainLayout->addChild(mEventsList);

	Screen::addOptionsMenuItem("First screen menu item 0", MAW_OPTIONS_MENU_ICON_CONSTANT_SAVE,true);
	Screen::addOptionsMenuItem("First screen menu item 1", MAW_OPTIONS_MENU_ICON_CONSTANT_SEARCH, true);
	this->addScreenListener(this);
	mClearOptions->addButtonListener(this);
}

/**
 * Destructor.
 */
FirstScreen::~FirstScreen() {
	this->removeScreenListener(this);
	mClearOptions->removeButtonListener(this);
}

/**
 * This method is called when the OptionsMenu is being closed
 * (either by the user canceling the menu with the back/menu
 * button, or when an item is selected.
 * @param Screen The screen that generated the event.
 */
void FirstScreen::optionsMenuClosed(Screen* screen) {

	ListViewItem* item = new ListViewItem();
	item->setText("First Screen event: Menu closed.");
	mEventsList->addChild(item);
}

/**
 * This method is called when an options menu item is selected.
 * @param index The index on which the item is placed into the
 * OptionsMenu.
 *  @param Screen The screen that generated the event.
 */
void FirstScreen::optionsMenuItemSelected(Screen* screen, int index) {
	ListViewItem* item = new ListViewItem();
	item->setText("First Screen event: Menu Item selected: index " + MAUtil::integerToString(index));
	mEventsList->addChild(item);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void FirstScreen::buttonClicked(Widget* button)
{
	if ( mClearOptions == (Button*) button)
	{
		if ( strcmp(mClearOptions->getText().c_str(), CLEAR_MENU ) == 0 )
		{
			Screen::removeOptionsMenu();
			mClearOptions->setText(RECREATE_MENU);
		}
		else
		{
			Screen::addOptionsMenuItem("First screen menu item 0", MAW_OPTIONS_MENU_ICON_CONSTANT_SAVE,true);
			Screen::addOptionsMenuItem("First screen menu item 1", MAW_OPTIONS_MENU_ICON_CONSTANT_SEARCH, true);
			mClearOptions->setText(CLEAR_MENU);
		}
	}

}
