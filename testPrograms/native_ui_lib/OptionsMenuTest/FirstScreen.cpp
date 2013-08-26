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
 * @file FirstScreen.cpp
 * @author emma
 */

#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MAHeaders.h"
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
	info->setText("First screen.2 Menu items with icons:save and search, one item with title only, and one item with custom image.");
	mMainLayout->addChild(info);

	mClearOptions = new Button();
	mClearOptions->setText(CLEAR_MENU);
	mMainLayout->addChild(mClearOptions);

	mEventsList = new ListView();
	mEventsList->fillSpaceHorizontally();
	mEventsList->fillSpaceVertically();
	mMainLayout->addChild(mEventsList);

	Screen::addOptionsMenuItem("First screen menu item 0", MAW_OPTIONS_MENU_ICON_CONSTANT_SAVE, false);
	Screen::addOptionsMenuItem("First screen menu item 1", MAW_OPTIONS_MENU_ICON_CONSTANT_SEARCH, false);
	Screen::addOptionsMenuItem("First screen menu item 2");
	Screen::addOptionsMenuItem("First screen menu item 3", RES_IMAGE);
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
			Screen::addOptionsMenuItem("First screen menu item 0", MAW_OPTIONS_MENU_ICON_CONSTANT_SAVE,false);
			Screen::addOptionsMenuItem("First screen menu item 1", MAW_OPTIONS_MENU_ICON_CONSTANT_SEARCH, false);
			Screen::addOptionsMenuItem("First screen menu item 2");
			Screen::addOptionsMenuItem("First screen menu item 3", RES_IMAGE);
			mClearOptions->setText(CLEAR_MENU);
		}
	}

}
