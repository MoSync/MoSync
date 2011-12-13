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
