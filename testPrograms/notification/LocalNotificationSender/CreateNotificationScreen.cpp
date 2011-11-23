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

// Text to label widgets
#define CONTENT_BODY_LABEL_TEXT "Content body:"
#define BADGE_NUMBER_LABEL_TEXT "Badge number:"
#define ALERT_ACTION_LABEL_TEXT "Alert action:"
#define PLAY_SOUND_LABEL_TEXT "Play sound:"
#define FIRE_TIME_LABEL_TEXT "Schedule in:"
#define SECONDS_LABEL_TEXT " seconds"
#define SEND_BUTTON_TEXT "SEND"

// Default text for edit box widgets
#define DEFAULT_CONTENT_BODY_EDIT_BOX_TEXT "Content body"
#define DEFAULT_BADGE_NUMBER_EDIT_BOX_TEXT "1"
#define DEFAULT_ALERT_ACTION_EDIT_BOX_TEXT "Alert action"
#define DEFAULT_FIRE_TIME_EDIT_BOX_TEXT "5"

// Default width for numeric edit box widgets.
#define NUMERIC_EDIT_BOX_WIDTH 70

// Used for verifying if a string can be converted to integer.
#define NINE 9
#define ZERO 0

// Text colors
#define TEXT_COLOR 0x000000
#define TEXT_COLOR_ERROR 0xFF0000

#include "CreateNotificationScreen.h"
#include "Util.h"
#include "MAHeaders.h"

/**
* Constructor.
*/
CreateNotificationScreen::CreateNotificationScreen():
	Screen(),
	mContentBody(NULL),
	mBadgeNumber(NULL),
	mAlertAction(NULL),
	mPlaySound(NULL),
	mTime(NULL),
	mCreateNotificationButton(NULL)
{
	this->createMainLayout();

	mContentBody->addEditBoxListener(this);
	mTime->addEditBoxListener(this);
	mCreateNotificationButton->addButtonListener(this);
	NotificationManager::getInstance()->addLocalNotificationListener(this);
	this->resetView();
}

/**
 * Destructor.
 */
CreateNotificationScreen::~CreateNotificationScreen()
{
	mContentBody->removeEditBoxListener(this);
	mTime->removeEditBoxListener(this);
	mCreateNotificationButton->removeButtonListener(this);

	if (isIOS())
	{
		mAlertAction->removeEditBoxListener(this);
		mBadgeNumber->removeEditBoxListener(this);
	}

	while (0 != mLocalNotificationVector.size())
	{
		LocalNotification* notification = mLocalNotificationVector[0];
		mLocalNotificationVector.remove(0);
		delete notification;
	}
	NotificationManager::getInstance()->removeLocalNotificationListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void CreateNotificationScreen::createMainLayout()
{
	VerticalLayout* mainLayout = new VerticalLayout();
	Screen::setMainWidget(mainLayout);

	ListView* listView = new ListView();
	listView->fillSpaceHorizontally();
	listView->fillSpaceVertically();
	mainLayout->addChild(listView);

	ListViewItem* listItem;
	HorizontalLayout* hLayout;
	Widget* space;
	Label* label;

	// ================ Content body =====================
	hLayout = new HorizontalLayout();
	label = new Label();
	label->setText(CONTENT_BODY_LABEL_TEXT);
	hLayout->addChild(label);

	space = this->createSpacer();
	hLayout->addChild(space);

	mContentBody = new EditBox();
	mContentBody->fillSpaceHorizontally();
	hLayout->addChild(mContentBody);

	listItem = new ListViewItem();
	listItem->addChild(hLayout);
	listView->addChild(listItem);

	if (isIOS())
	{
		// ================ Badge number =====================
		hLayout = new HorizontalLayout();
		label = new Label();
		label->setText(BADGE_NUMBER_LABEL_TEXT);
		hLayout->addChild(label);

		space = this->createSpacer();
		hLayout->addChild(space);

		mBadgeNumber = new EditBox();
		mBadgeNumber->setWidth(NUMERIC_EDIT_BOX_WIDTH);
		mBadgeNumber->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
		mBadgeNumber->addEditBoxListener(this);
		hLayout->addChild(mBadgeNumber);

		listItem = new ListViewItem();
		listItem->addChild(hLayout);
		listView->addChild(listItem);

		// ================ Alert action =====================
		hLayout = new HorizontalLayout();
		label = new Label();
		label->setText(ALERT_ACTION_LABEL_TEXT);
		hLayout->addChild(label);

		space = this->createSpacer();
		hLayout->addChild(space);

		mAlertAction = new EditBox();
		mAlertAction->fillSpaceHorizontally();
		mAlertAction->addEditBoxListener(this);
		hLayout->addChild(mAlertAction);

		listItem = new ListViewItem();
		listItem->addChild(hLayout);
		listView->addChild(listItem);
	}
	// ================ Play sound =====================
	hLayout = new HorizontalLayout();
	label = new Label();
	label->setText(PLAY_SOUND_LABEL_TEXT);
	hLayout->addChild(label);

	space = this->createSpacer();
	hLayout->addChild(space);

	mPlaySound = new CheckBox();
	hLayout->addChild(mPlaySound);

	listItem = new ListViewItem();
	listItem->addChild(hLayout);
	listView->addChild(listItem);

	// ================ Fire time =====================
	hLayout = new HorizontalLayout();
	label = new Label();
	label->setText(FIRE_TIME_LABEL_TEXT);
	label->wrapContentHorizontally();
	hLayout->addChild(label);

	space = this->createSpacer();
	hLayout->addChild(space);

	mTime = new EditBox();
	mTime->setWidth(NUMERIC_EDIT_BOX_WIDTH);
	mTime->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mTime->addEditBoxListener(this);
	hLayout->addChild(mTime);

	label = new Label();
	label->setText(SECONDS_LABEL_TEXT);
	label->wrapContentHorizontally();
	hLayout->addChild(label);

	listItem = new ListViewItem();
	listItem->addChild(hLayout);
	listView->addChild(listItem);

//	// ================= Send image button ================
	hLayout = new HorizontalLayout();
	hLayout->fillSpaceHorizontally();
	listItem = new ListViewItem();
	listItem->addChild(hLayout);
	listView->addChild(listItem);
	mCreateNotificationButton = new ImageButton();
	mCreateNotificationButton->setImage(RES_IMAGE);

	space = this->createSpacer();
	hLayout->addChild(space);
	hLayout->addChild(mCreateNotificationButton);

	// Align the image button at the center of the layout.
	// Only after adding the image button widget to layout the widget's width
	// is available.
	int spacerWidth = hLayout->getWidth() / 2 -
		mCreateNotificationButton->getWidth() / 2;
	space->setWidth(spacerWidth);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void CreateNotificationScreen::buttonClicked(Widget* button)
{
	printf("CreateNotificationScreen::buttonClicked");
	if (button == mCreateNotificationButton)
	{
		if (!this->isUserInputDataValid())
		{
			return;
		}

		LocalNotification* notification = new LocalNotification();
		mLocalNotificationVector.add(notification);

		// Set fire date.
		String secondsString = mTime->getText();
		int seconds = MAUtil::stringToInteger(secondsString);
		int secondsLocalTime = maLocalTime();
		int scheduleTime = secondsLocalTime + seconds;
		tm fireDate;
		split_time(scheduleTime, &fireDate);
		notification->setFireDate(&fireDate);

		// Set content body
		MAUtil::String contentBody = mContentBody->getText();
		notification->setContentBody(contentBody);

		if (isIOS())
		{
			// Set badge number
			MAUtil::String badgeNumberString = mBadgeNumber->getText();
			int badgeNumber = MAUtil::stringToInteger(badgeNumberString, 10);
			notification->setBadgeNumber(badgeNumber);

			// Set alert action.
			MAUtil::String alertAction = mAlertAction->getText();
			notification->setAlertAction(alertAction);
		}

		// Set play sound property
		bool playSound = mPlaySound->isChecked();
		notification->setPlaySound(playSound);

		NotificationManager::getInstance()->scheduleLocalNotification(notification);
		printf("notification created");
		this->resetView();
	}
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void CreateNotificationScreen::editBoxReturn(EditBox* editBox)
{
	editBox->hideKeyboard();
}

/**
 * Called when the application receives a local notification.
 * @param localNotification The received local notification.
 */
void CreateNotificationScreen::didReceiveLocalNotification(
    LocalNotification& localNotification)
{
	printf("CreateNotificationScreen::didReceiveLocalNotification");
	for (int i = 0; i < mLocalNotificationVector.size(); i++)
	{
		LocalNotification* storedNotification = mLocalNotificationVector[i];
		if (localNotification.getHandle() == storedNotification->getHandle())
		{
			printf("delete local notification");
			mLocalNotificationVector.remove(i);
			delete storedNotification;
			break;
		}
	}
}

/**
 * Reset view's content.
 * Set default values for widgets.
 */
void CreateNotificationScreen::resetView()
{
	mContentBody->setText(DEFAULT_CONTENT_BODY_EDIT_BOX_TEXT);
	mPlaySound->setState(false);
	mTime->setText(DEFAULT_FIRE_TIME_EDIT_BOX_TEXT);
	mTime->setFontColor(TEXT_COLOR);

	if (isIOS())
	{
		mBadgeNumber->setText(DEFAULT_BADGE_NUMBER_EDIT_BOX_TEXT);
		mBadgeNumber->setFontColor(TEXT_COLOR);
		mAlertAction->setText(DEFAULT_ALERT_ACTION_EDIT_BOX_TEXT);
	}
}

/**
 * Checks if the user input data is valid(e.g. numeric edit box values
 * can be converted to integer).
 * @return True if the data is valid, false otherwise.
 */
bool CreateNotificationScreen::isUserInputDataValid()
{
	if (isIOS())
	{
		String badgeNumber = mBadgeNumber->getText();
		if (!this->canStringBeConvertedToInteger(badgeNumber))
		{
			printf("invalid badge number value");
			mBadgeNumber->setFontColor(TEXT_COLOR_ERROR);
			return false;
		}
	}

	String time = mTime->getText();
	if (!this->canStringBeConvertedToInteger(time))
	{
		mTime->setFontColor(TEXT_COLOR_ERROR);
		return false;
	}
	return true;
}

/**
 * Creates an empty widget with a given width.
 * The ownership of the result is passed to the caller.
 * @return An empty widget.
 */
Widget* CreateNotificationScreen::createSpacer(const int width)
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
bool CreateNotificationScreen::canStringBeConvertedToInteger(
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
