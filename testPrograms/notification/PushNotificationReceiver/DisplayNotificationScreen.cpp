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
 * @file DisplayNotificationScreen.cpp
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Screen used for displaying push notification's values.
 */

#define DEFAULT_ALERT_MESSAGE_TEXT "Message: "
#define DEFAULT_SOUND_TEXT "Sound: "
#define DEFAULT_BADGE_NUMBER_TEXT "Badge number: "
#define APPLICATION_REGISTERED "Application has registered for push notifications"
#define APPLICATION_CANNOT_REGISTER "Application did not register for push notifications"

// Default width for numeric edit box widgets.
#define NUMERIC_EDIT_BOX_WIDTH 70

// Used for verifying if a string can be converted to integer.
#define NINE 9
#define ZERO 0

// Text colors
#define TEXT_COLOR 0x000000
#define TEXT_COLOR_ERROR 0xFF0000

#define SCREEN_TITLE_NAME "Push notification"

#include <maapi.h>
#include <MAUtil/util.h>
#include <Notification/PushNotification.h>

#include "DisplayNotificationScreen.h"
#include "Util.h"

/**
* Constructor.
*/
DisplayNotificationScreen::DisplayNotificationScreen():
	Screen(),
	mMessageLabel(NULL),
	mSoundFileNameLabel(NULL)
{
	this->setTitle(SCREEN_TITLE_NAME);
	this->createMainLayout();
}

/**
 * Destructor.
 */
DisplayNotificationScreen::~DisplayNotificationScreen()
{
}

/**
 * This screen is notified when registration is done.
 * @param registrationStatus True if registration succeed, false if
 * it failed.
 */
void DisplayNotificationScreen::pushRegistrationDone(bool registrationStatus)
{
	if ( registrationStatus )
		mMessageLabel->setText(APPLICATION_REGISTERED);
	else
		mMessageLabel->setText(APPLICATION_CANNOT_REGISTER);

}

/**
 * Displays notification's content on the screen.
 */
void DisplayNotificationScreen::pushNotificationReceived(
	Notification::PushNotification& pushNotification)
{
	if (pushNotification.containsMessage())
	{
		MAUtil::String message = DEFAULT_ALERT_MESSAGE_TEXT;
		message += pushNotification.getMessage()->c_str();
		mMessageLabel->setText(message);
	}

	if (isIOS())
	{
		if (pushNotification.containsSoundFileName())
		{
			MAUtil::String sound = DEFAULT_SOUND_TEXT;
			sound += pushNotification.getSoundFileName()->c_str();
			mSoundFileNameLabel->setText(sound);
		}

		if (pushNotification.containsIconBadgeNumber())
		{
			MAUtil::String badgeNumber = DEFAULT_BADGE_NUMBER_TEXT;
			badgeNumber += MAUtil::integerToString(
				pushNotification.getIconBadgeNumebr());
			mBadgeNumberLabel->setText(badgeNumber);
		}
	}
}

/**
 * Helper function to create list view item with
 * specific label and edit box.
 */
ListViewItem* DisplayNotificationScreen::createListViewItem(
	const MAUtil::String& labelText, Widget* widget)
{
	ListViewItem* listItem;
	HorizontalLayout* hLayout;
	Widget* space;
	Label* label;

	hLayout = new HorizontalLayout();
	label = new Label();
	label->setText(labelText);
	hLayout->addChild(label);

	space = this->createSpacer();
	hLayout->addChild(space);

	widget->fillSpaceHorizontally();
	hLayout->addChild(widget);

	listItem = new ListViewItem();
	listItem->addChild(hLayout);
	return listItem;
}

/**
 * Creates and adds main layout to the screen.
 */
void DisplayNotificationScreen::createMainLayout()
{
	RelativeLayout* mainLayout = new RelativeLayout();
	Screen::setMainWidget(mainLayout);

    // Get the screen size.
    MAExtent screenSize = maGetScrSize();
    int screenWidth = EXTENT_X(screenSize);
    int screenHeight = EXTENT_Y(screenSize);

	printf("screen height = %d",screenHeight);
	printf("screen WIDTH = %d",screenWidth);
	// Only 3 widgets for this screen.
	int widgetHeight = screenHeight / 3;
	int top = 0, left = 0;

	// Add message label
	mMessageLabel = new Label();
	mMessageLabel->setSize(screenWidth, widgetHeight);
	mMessageLabel->setPropertyInt(MAW_WIDGET_TOP, top);
	mMessageLabel->setFontColor(0xFF0000);
	mMessageLabel->setPropertyInt(MAW_WIDGET_LEFT, left);
	mMessageLabel->setText(DEFAULT_ALERT_MESSAGE_TEXT);
	mainLayout->addChild(mMessageLabel);
	top += widgetHeight;

	if (isIOS())
	{
		// Add sound file name label.
		mSoundFileNameLabel = new Label();
		mSoundFileNameLabel->setSize(screenWidth, widgetHeight);
		mSoundFileNameLabel->setPropertyInt(MAW_WIDGET_TOP, top);
		mSoundFileNameLabel->setPropertyInt(MAW_WIDGET_LEFT, left);
		mSoundFileNameLabel->setText(DEFAULT_SOUND_TEXT);
		mainLayout->addChild(mSoundFileNameLabel);
		top += widgetHeight;

		// Add icon badge number label.
		mBadgeNumberLabel = new Label();
		mBadgeNumberLabel->setSize(screenWidth, widgetHeight);
		mBadgeNumberLabel->setPropertyInt(MAW_WIDGET_TOP, top);
		mBadgeNumberLabel->setPropertyInt(MAW_WIDGET_LEFT, left);
		mBadgeNumberLabel->setText(DEFAULT_BADGE_NUMBER_TEXT);
		mainLayout->addChild(mBadgeNumberLabel);
		top += widgetHeight;
	}

}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void DisplayNotificationScreen::buttonClicked(Widget* button)
{
	printf("DisplayNotificationScreen::buttonClicked");
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void DisplayNotificationScreen::editBoxReturn(EditBox* editBox)
{
	editBox->hideKeyboard();
}

/**
 * Reset view's content.
 * Set default values for widgets.
 */
void DisplayNotificationScreen::resetView()
{
}

/**
 * Checks if the user input data is valid(e.g. numeric edit box values
 * can be converted to integer).
 * @return True if the data is valid, false otherwise.
 */
bool DisplayNotificationScreen::isUserInputDataValid()
{
}

/**
 * Creates an empty widget with a given width.
 * The ownership of the result is passed to the caller.
 * @return An empty widget.
 */
Widget* DisplayNotificationScreen::createSpacer(const int width)
{
	HorizontalLayout* spacer = new HorizontalLayout();
	spacer->setWidth(width);
	return spacer;
}

/**
 * Checks if a given string can be converted to integer.
 * @return True if string contains only characters between [0-9], false
 * otherwise.
 */
bool DisplayNotificationScreen::canStringBeConvertedToInteger(
	const MAUtil::String& string)
{
	for (int i = 0;i < string.length(); i++)
	{
		char character = string[i] - '0';
		if (ZERO > character || NINE < character)
		{
			return false;
		}
	}

	return true;
}
