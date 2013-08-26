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
 * @file FlashModeScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a list filled with available flash mode
 * constants.
 * The user can change current flash mode by selecting a list item.
 */

#define SCREEN_TITLE "Select flash mode"

#define FLASH_MODE_AUTO_TEXT "Flash mode auto"
#define FLASH_MODE_ON_TEXT "Flash mode on"
#define FLASH_MODE_OFF_TEXT "Flash mode off"

#define SELECTED_QUALITY_COLOR 0x123456
#define UNSELECTED_QUALITY_COLOR 0xFFFFFF

#include <maapi.h>
#include <conprint.h>
#include <MAUtil/String.h>
#include <MAUtil/util.h>

#include "FlashModeScreen.h"

/**
 * Constructor.
 */
FlashModeScreen::FlashModeScreen():
	Screen(),
	mListView(NULL),
	mFlashAutoListItem(NULL),
	mFlashOnListItem(NULL),
	mFlashOffListItem(NULL)
{
	this->setTitle(SCREEN_TITLE);
	this->createMainLayout();

	mListView->addListViewListener(this);
}

/**
 * Destructor.
 */
FlashModeScreen::~FlashModeScreen()
{
	mListView->removeListViewListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void FlashModeScreen::createMainLayout()
{
	mListView = new ListView();
	Screen::setMainWidget(mListView);

	Label* label;

	mFlashAutoListItem = new ListViewItem();
	mListView->addChild(mFlashAutoListItem);
	mFlashAutoListItem->setText(FLASH_MODE_AUTO_TEXT);

	mFlashOnListItem = new ListViewItem();
	mListView->addChild(mFlashOnListItem);
	mFlashOnListItem->setText(FLASH_MODE_ON_TEXT);

	mFlashOffListItem = new ListViewItem();
	mListView->addChild(mFlashOffListItem);
	mFlashOffListItem->setText(FLASH_MODE_OFF_TEXT);

	// Set the default value.
	mFlashAutoListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
	mSelectedListItem = mFlashAutoListItem;
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void FlashModeScreen::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
	if ( mListView == listView )
	{
		mFlashAutoListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
		mFlashOnListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
		mFlashOffListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
		int selectedFlashMode;

		if (listViewItem == mFlashAutoListItem)
		{
			selectedFlashMode = MA_CAPTURE_FLASH_AUTO;
			mFlashAutoListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
		}
		else if (listViewItem == mFlashOnListItem)
		{
			selectedFlashMode = MA_CAPTURE_FLASH_ON;
			mFlashOnListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
		}
		else if (listViewItem == mFlashOffListItem)
		{
			selectedFlashMode = MA_CAPTURE_FLASH_OFF;
			mFlashOffListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
		}

		// Set the selected video quality.
		String value = MAUtil::integerToString(selectedFlashMode);
		int syscallResult = maCaptureSetProperty(
			MA_CAPTURE_FLASH,
			value.c_str());
		printf("maCaptureSetProperty(MA_CAPTURE_FLASH) - %d", syscallResult);
	}
}
