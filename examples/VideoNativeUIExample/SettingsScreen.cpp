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
 * @author Emma Tresanszki
 *
 * The screen that handles settings, like:
 *  - Display video file duration.
 *  - The default link/path to load at application startup.
 *  - Reload the default link.
 */

// Include application's Moblet class.
#include "Moblet.h"

#include "Util.h"
#include "SettingsScreen.h"

/**
 * Constructor.
 */
SettingsScreen::SettingsScreen():
	Screen(),
	mListView(NULL),
	mDisplayDuration(NULL),
	mHomeUrl(NULL),
	mReloadItem(NULL)
{
	// Initialize the UI and set the listeners.

	setTitle("Settings");

	createMainLayout();

	mListView->addListViewListener(this);
	mDisplayDuration->addCheckBoxListener(this);
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	mListView->removeListViewListener(this);
	mDisplayDuration->removeCheckBoxListener(this);
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void SettingsScreen::listViewItemClicked(
	ListView* listView, ListViewItem* listViewItem)
{
	if ( mListView == listView &&  mReloadItem == listViewItem )
	{
		// Reload the default source in VideoView.
	}
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void SettingsScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	if ( mDisplayDuration == checkBox )
	{
		// For the moment we are not interested in this event, as the
		// state of the check box is queried at tab change only.
	}

}

/**
 * Get the home url that is set in the list view item.
 */
MAUtil::String SettingsScreen::getDefaultUrl()
{
	return mHomeUrl->getText();
}

/**
 * Get the checked state of the duration setting item.
 * @return true if it is checked, false otherwise.
 */
bool SettingsScreen::getDurationSettingValue()
{
	return mDisplayDuration->isChecked();
}

/**
 * Creates and adds main layout to the screen.
 */
void SettingsScreen::createMainLayout()
{
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(SEA_GREEN);
	setMainWidget(mMainLayout);

	mListView = new ListView();
	mMainLayout->addChild(mListView);

	// First setting item contains a check box and a label.
	ListViewItem* durationItem = new ListViewItem();
	durationItem->setText(DISPLAY_DURATION);
	HorizontalLayout* durationItemLayout = new HorizontalLayout();
	durationItemLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	mDisplayDuration = new CheckBox();
	mDisplayDuration->setState(true);
	durationItemLayout->addChild(mDisplayDuration);
	durationItem->addChild(durationItemLayout);

	mListView->addChild(durationItem);

	// Add an item with the default homepage.
	ListViewItem* homeUrlItem = new ListViewItem();
	homeUrlItem->setWidth(getScreenWidth());
	VerticalLayout* homeUrlLayout = new VerticalLayout();
	Label* hintLabel = new Label();
	hintLabel->setText(HOME_URL);
	hintLabel->setFontColor(DARK_SEA_GREEN);
	homeUrlLayout->addChild(hintLabel);
	mHomeUrl = new Label();
	mHomeUrl->setMaxNumberOfLines(2);
	mHomeUrl->setWidth(getScreenWidth());
	setDefaultHomepage();
	homeUrlLayout->addChild(mHomeUrl);
	homeUrlItem->addChild(homeUrlLayout);
	mListView->addChild(homeUrlItem);

	mReloadItem = new ListViewItem();
	mReloadItem->setText(RESET_TO_DEFAULT);
	mListView->addChild(mReloadItem);
}

/**
 * Set the default value for the homepage.
 * It is different depending on the platform, as they can play different
 * media formats.
 */
void SettingsScreen::setDefaultHomepage()
{
	// Default urls depending on the platform.
	if ( isAndroid() )
	{
		mHomeUrl->setText(HOME_URL_ANDROID);
	}
	else
	{
		mHomeUrl->setText(HOME_URL_IOS);
	}
}

ListView* SettingsScreen::getSettingsListView()
{
	return mListView;
}
ListViewItem* SettingsScreen::getResetListViewItem()
{
	return mReloadItem;
}
