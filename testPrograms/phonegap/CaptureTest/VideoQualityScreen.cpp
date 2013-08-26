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
#include "Util.h"

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

	if ( isIOS() )
	{
		mQualityMediumListItem = new ListViewItem();
		mListView->addChild(mQualityMediumListItem);
		mQualityMediumListItem->setText(VIDEO_QUALITY_MEDIUM_TEXT);
	}

	mQualityHighListItem = new ListViewItem();
	mListView->addChild(mQualityHighListItem);
	mQualityHighListItem->setText(VIDEO_QUALITY_HIGH_TEXT);

	// Set and highlight the default value.
	if ( isIOS() )
	{
		highlightDefaultSetting(mQualityMediumListItem);
	}
	else
	{
		highlightDefaultSetting(mQualityHighListItem);
	}
}

/**
 * Select and highlight a default quality value.
 */
void VideoQualityScreen::highlightDefaultSetting(ListViewItem* item)
{
	item->setBackgroundColor(SELECTED_QUALITY_COLOR);
	mSelectedListItem = item;
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
	if ( mListView == listView )
	{
		mQualityHighListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
		if ( isIOS() )
		{
			mQualityMediumListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
		}
		mQualityLowListItem->setBackgroundColor(UNSELECTED_QUALITY_COLOR);
		int selectedQuality;

		if (listViewItem == mQualityLowListItem)
		{
			selectedQuality = MA_CAPTURE_VIDEO_QUALITY_LOW;
			mQualityLowListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
		}
		else if (listViewItem == mQualityHighListItem)
		{
			selectedQuality = MA_CAPTURE_VIDEO_QUALITY_HIGH;
			mQualityHighListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
		}
		else if (listViewItem == mQualityMediumListItem)
		{
			selectedQuality = MA_CAPTURE_VIDEO_QUALITY_MEDIUM;
			mQualityMediumListItem->setBackgroundColor(SELECTED_QUALITY_COLOR);
		}

		// Set the selected video quality.
		String value = MAUtil::integerToString(selectedQuality);
		int syscallResult = maCaptureSetProperty(
			MA_CAPTURE_VIDEO_QUALITY,
			value.c_str());
		printf("maCaptureSetProperty(MA_CAPTURE_VIDEO_QUALITY) - %d", syscallResult);
	}
}
