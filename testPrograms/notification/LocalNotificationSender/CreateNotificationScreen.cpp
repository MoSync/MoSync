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
#define CONTENT_TITLE_LABEL_TEXT "Content title:"
#define TICKER_TEXT_LABEL_TEXT "Ticker text:"
#define BADGE_NUMBER_LABEL_TEXT "Badge number:"
#define ALERT_ACTION_LABEL_TEXT "Alert action:"
#define PLAY_SOUND_LABEL_TEXT "Play sound:"
#define SOUND_PATH_LABEL_TEXT "Sound path:"
#define VIBRATE_LABEL_TEXT "Vibration:"
#define VIBRATE_DURATION_LABEL_TEXT "Vibration duration(seconds):"
#define FLASH_LABEL_TEXT "Flashing: "
#define FLASH_COLOR_LABEL_TEXT "Flash color:"
#define FLASH_ON_LABEL_TEXT "Flash On for:(seconds)"
#define FLASH_OFF_LABEL_TEXT "Flash Off for:(seconds)"
#define FIRE_TIME_LABEL_TEXT "Schedule in(seconds):"
#define SECONDS_LABEL_TEXT " seconds"
#define SEND_BUTTON_TEXT "SEND"

// Default text for edit box widgets
#define DEFAULT_CONTENT_BODY_EDIT_BOX_TEXT "Content body"
#define DEFAULT_CONTENT_TITLE_EDIT_BOX_TEXT "Content title"
#define DEFAULT_TICKER_TEXT_EDIT_BOX_TEXT "Ticker text"
#define DEFAULT_SOUND_PATH_EDIT_BOX_TEXT "file:///sdcard/notification/ringer.mp3"
#define DEFAULT_VIBRATION_EDIT_BOX_TEXT "" // 0 duration
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
	mContentTitle(NULL),
	mTickerText(NULL),
	mBadgeNumber(NULL),
	mAlertAction(NULL),
	mPlaySound(NULL),
	mSoundPath(NULL),
	mVibrate(NULL),
	mVibrateDuration(NULL),
	mFlash(NULL),
	mFlashColor(NULL),
	mFlashOnLength(NULL),
	mFlashOffLength(NULL),
	mTime(NULL),
	mCreateNotificationButton(NULL)
{
	this->createMainLayout();

	mContentBody->addEditBoxListener(this);
	mTime->addEditBoxListener(this);
	mCreateNotificationButton->addButtonListener(this);
	NotificationManager::getInstance()->addLocalNotificationListener(this);
//	this->resetView();
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
 * Helper function to create list view item with
 * specific label and edit/check box.
 */
ListViewItem* CreateNotificationScreen::createListViewItem(const MAUtil::String& labelText, Widget* widget)//EditBox* editBox)
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

	// ================ Content body =====================
	mContentBody = new EditBox();
	listView->addChild(createListViewItem(CONTENT_BODY_LABEL_TEXT,(Widget*) mContentBody));

	if ( isAndroid() )
	{
		// ================ Content title =====================
		mContentTitle = new EditBox();
		listView->addChild(createListViewItem(CONTENT_TITLE_LABEL_TEXT, mContentTitle));

		// ================ Ticker text =====================
		mTickerText = new EditBox();
		listView->addChild(createListViewItem(TICKER_TEXT_LABEL_TEXT, mTickerText));
	}
	else
	{
		// ================ Badge number =====================
		mBadgeNumber = new EditBox();
		mBadgeNumber->setWidth(NUMERIC_EDIT_BOX_WIDTH);
		mBadgeNumber->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
		mBadgeNumber->addEditBoxListener(this);
		listView->addChild(createListViewItem(BADGE_NUMBER_LABEL_TEXT, mBadgeNumber));

		// ================ Alert action =====================
		mAlertAction = new EditBox();
		mAlertAction->addEditBoxListener(this);
		listView->addChild(createListViewItem(ALERT_ACTION_LABEL_TEXT, mAlertAction));
	}

	// ================ Play sound =====================
	mPlaySound = new CheckBox();
	listView->addChild(createListViewItem(PLAY_SOUND_LABEL_TEXT, mPlaySound));
	printf("after play sound");

	if ( isAndroid() )
	{
		// ================ Sound path=====================
		mSoundPath = new EditBox();
		listView->addChild(createListViewItem(SOUND_PATH_LABEL_TEXT, mSoundPath));
		printf("after create sound path");

		// ================ Vibrate =====================
		mVibrate = new CheckBox();
		listView->addChild(createListViewItem(VIBRATE_LABEL_TEXT, mVibrate));

		mVibrateDuration = new EditBox();
		mVibrateDuration->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
		listView->addChild(createListViewItem(VIBRATE_DURATION_LABEL_TEXT, mVibrateDuration));

		// ================ Flashing LED =====================
		mFlash = new CheckBox();
		listView->addChild(createListViewItem(FLASH_LABEL_TEXT, mFlash));
		// The pattern
		mFlashColor = new EditBox();
		listView->addChild(createListViewItem(FLASH_COLOR_LABEL_TEXT, mFlashColor));
		mFlashOnLength = new EditBox();
		mFlashOnLength->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
		listView->addChild(createListViewItem(FLASH_ON_LABEL_TEXT, mFlashOnLength));
		mFlashOffLength = new EditBox();
		mFlashOffLength->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
		listItem->addChild(createListViewItem(FLASH_OFF_LABEL_TEXT, mFlashOffLength));
	}

	// ================ Fire time =====================
	mTime = new EditBox();
	mTime->setWidth(NUMERIC_EDIT_BOX_WIDTH);
	mTime->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	listView->addChild(createListViewItem(FIRE_TIME_LABEL_TEXT, mTime));

	// ================= Send image button ================
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

		// Set play sound property
		bool playSound = mPlaySound->isChecked();
		notification->setPlaySound(playSound);

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
		else
		{
			notification->setContentTitle(mContentTitle->getText());
			notification->setTickerText(mTickerText->getText());
			if ( mVibrate->isChecked() )
			{
				notification->setVibrate(true);
				if ( mVibrateDuration->getText().length() > 0 )
				{
					notification->setVibrateDuration(
							MAUtil::stringToInteger(mVibrateDuration->getText()));
				}
			}
			else
			{
				notification->setVibrate(false);
			}
			if ( mFlash->isChecked() )
			{
				notification->setFlashLights(true);
				if ( mFlashColor->getText().length() > 0
						&&
						mFlashOnLength->getText().length() > 0
						&&
						mFlashOffLength->getText().length() > 0)
				{
					struct NotificationFlashLights pattern = NotificationFlashLights(
							MAUtil::stringToInteger(mFlashColor->getText()),
							MAUtil::stringToInteger(mFlashOnLength->getText()),
							MAUtil::stringToInteger(mFlashOffLength->getText()));
					notification->setFlashLightsPattern(pattern);
				}
			}
			else
			{
				notification->setFlashLights(false);
			}
			if ( playSound )
			{
				notification->setSound(mSoundPath->getText());
			}
		}

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
	else
	{
		mContentTitle->setText(DEFAULT_CONTENT_TITLE_EDIT_BOX_TEXT);
		mTickerText->setText(DEFAULT_TICKER_TEXT_EDIT_BOX_TEXT);
		mSoundPath->setText(DEFAULT_SOUND_PATH_EDIT_BOX_TEXT);
		mVibrate->setState(false);
		mVibrateDuration->setText(DEFAULT_VIBRATION_EDIT_BOX_TEXT);
		mFlash->setState(false);
		mFlashColor->setText("");
		mFlashOnLength->setText("");
		mFlashOffLength->setText("");
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
	else
	{
		if ( mVibrate->isChecked() )
		{
			String vibrationDuration = mVibrateDuration->getText();
			if (!this->canStringBeConvertedToInteger(vibrationDuration))
			{
				printf("invalid vibration duration value");
				mVibrateDuration->setFontColor(TEXT_COLOR_ERROR);
				return false;
			}
		}
		if ( mFlash->isChecked() )
		{
			String flashColor = mFlashColor->getText();
			if (!this->canStringBeConvertedToColor(flashColor))
			{
				printf("invalid flash color value");
				mFlashColor->setFontColor(TEXT_COLOR_ERROR);
				return false;
			}
			String flashOn = mFlashOnLength->getText();
			if (!this->canStringBeConvertedToInteger(flashOn))
			{
				printf("invalid flash on duration value");
				mFlashOnLength->setFontColor(TEXT_COLOR_ERROR);
				return false;
			}
			String flashOff = mFlashOffLength->getText();
			if (!this->canStringBeConvertedToInteger(flashOff))
			{
				printf("invalid flash off duration value");
				mFlashOffLength->setFontColor(TEXT_COLOR_ERROR);
				return false;
			}
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

/**
 * Checks if the given string can be converted to a color.
 * @return True if the string is in the format 0xRRGGBB.
 */
bool CreateNotificationScreen::canStringBeConvertedToColor(const MAUtil::String& string)
{
	if ( string.length() == 8)
	{
		if ( 0 == MAUtil::lowerString(string).find("0x",0) )
		{
			return true;
		}
		else
			return false;
	}
	return false;
}
