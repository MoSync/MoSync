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
 * @file SettingsScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a list filled with available video quality
 * constants.
 * The user can change current video quality by selecting a list item.
 */

#define SCREEN_TITLE "Select video quality"

#define VIDEO_QUALITY_LOW_TEXT "Video quality low"
#define VIDEO_QUALITY_MEDIUM_TEXT "Video quality medium"
#define VIDEO_QUALITY_HIGH_TEXT "Video quality high"

#define SELECTED_QUALITY_COLOR 0x123456
#define UNSELECTED_QUALITY_COLOR 0xFFFFFF

#include <maapi.h>
#include <conprint.h>
#include <MAUtil/String.h>
#include <MAUtil/util.h>

#include "VideoQualityScreen.h"

/**
 * Constructor.
 */
VideoQualityScreen::VideoQualityScreen():
	Screen(),
	mListView(NULL),
	mQualityLowListItem(NULL)
{
	this->setTitle(SCREEN_TITLE);
	this->createMainLayout();

	mListView->addListViewListener(this);
}

/**
 * Destructor.
 */
VideoQualityScreen::~VideoQualityScreen()
{
	mListView->removeListViewListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void VideoQualityScreen::createMainLayout()
{
	mListView = new ListView();
	Screen::setMainWidget(mListView);

	Label* label;

	mQualityLowListItem = new ListViewItem();
	mListView->addChild(mQualityLowListItem);
	mQualityLowListItem->setText(VIDEO_QUALITY_LOW_TEXT);

	mQualityMediumListItem = new ListViewItem();
	mListView->addChild(mQualityMediumListItem);
	mQualityMediumListItem->setText(VIDEO_QUALITY_MEDIUM_TEXT);

	mQualityHighListItem = new ListViewItem();
	mListView->addChild(mQualityHighListItem);
	mQualityHighListItem->setText(VIDEO_QUALITY_HIGH_TEXT);

	// Set the default value.
	mQualityMediumListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
	mSelectedListItem = mQualityMediumListItem;
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void VideoQualityScreen::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
	mQualityHighListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
	mQualityMediumListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
	mQualityLowListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
	int selectedQuality;

	if (listViewItem == mQualityLowListItem)
	{
		selectedQuality = MA_CAPTURE_VIDEO_QUALITY_LOW;
		mQualityLowListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
	}
	else if (listViewItem == mQualityMediumListItem)
	{
		selectedQuality = MA_CAPTURE_VIDEO_QUALITY_MEDIUM;
		mQualityMediumListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
	}
	else if (listViewItem == mQualityHighListItem)
	{
		selectedQuality = MA_CAPTURE_VIDEO_QUALITY_HIGH;
		mQualityHighListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
	}

	// Set the selected video quality.
	String value = MAUtil::integerToString(selectedQuality);
	int syscallResult = maCaptureSetProperty(
		MA_CAPTURE_VIDEO_QUALITY,
		value.c_str());
	printf("maCaptureSetProperty(MA_CAPTURE_VIDEO_QUALITY) - %d", syscallResult);
}
