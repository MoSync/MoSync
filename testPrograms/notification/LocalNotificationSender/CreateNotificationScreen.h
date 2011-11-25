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
 * @file CreateNotificationScreen.h
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Screen used for creating notifications.
 * Contains widgets(e.g. edit box, check box, button) used for setting
 * notification values.
 */

#ifndef CREATENOTIFICATIONSCREEN_H_
#define CREATENOTIFICATIONSCREEN_H_

#define DEFAULT_SPACER_WIDTH 10

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <MAUtil/String.h>

#include <Notification/NotificationManager.h>
#include <Notification/LocalNotification.h>
#include <Notification/LocalNotificationListener.h>

using namespace NativeUI;
using namespace MAUtil;
using namespace Notification;


/**
 * Screen used for creating notifications.
 * Contains widgets(e.g. edit box, check box, button) used for setting
 * notification values.
 */
class CreateNotificationScreen:
	public Screen,
	public ButtonListener,
	public EditBoxListener,
	public LocalNotificationListener
{
public:
	/**
	 * Constructor.
	 */
	CreateNotificationScreen();

	/**
	 * Destructor.
	 */
	~CreateNotificationScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Helper function to create list view item with
	 * specific label and edit/check box.
	 */
	ListViewItem* createListViewItem(const MAUtil::String& labelText, Widget* widget);//EditBox* editBox);

	/**
	 * Reset view's content.
	 * Set default values for widgets.
	 */
	virtual void resetView();

	/**
	 * Checks if the user input data is valid(e.g. numeric edit box values
	 * can be converted to integer).
	 * @return True if the data is valid, false otherwise.
	 */
	virtual bool isUserInputDataValid();

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

    /**
     * Called when the application receives a local notification.
     * @param localNotification The received local notification.
     */
    virtual void didReceiveLocalNotification(
        LocalNotification& localNotification);

    /**
     * Creates an empty widget with a given width.
     * The ownership of the result is passed to the caller.
     * @return An empty widget.
     */
    virtual Widget* createSpacer(const int width = DEFAULT_SPACER_WIDTH);

    /**
     * Checks if a given string can be converted to integer.
     * @return True if string contains only characters between [0-9], false
     * otherwise.
     */
    virtual bool canStringBeConvertedToInteger(const MAUtil::String& string);

private:
    /**
     * Used for getting notification's content body.
     */
    EditBox* mContentBody;

    /**
     * Used for getting notification's content title.
     * Available only on Android platform.
     */
    EditBox* mContentTitle;

    /**
     * Used for getting the ticker text.
     * Available only on Android platform.
     */
    EditBox* mTickerText;

    /**
     * Used for getting notification's badge number.
     * Shown only on iOS platform.
     */
    EditBox* mBadgeNumber;

    /**
     * Used for getting notification's alert message.
     * Shown only on iOS platform.
     */
    EditBox* mAlertAction;

    /**
     * If set the notification will play sound.
     * If set to true it will play the default sound.
     */
    CheckBox* mPlaySound;

    /**
     * Used for the sound that will be played.
     * Available only on Android platform.
     */
    EditBox* mSoundPath;

    /**
     * If set the notification will alert the user with a vibration.
     * If no vibrate duration is set, the default vibration pattern
     * will be used.
     * Available only on Android platform.
     */
    CheckBox* mVibrate;

    /**
     * Sets the vibration duration.
     * Available only on Android platform.
     */
    EditBox* mVibrateDuration;

    /**
     * If set the notification will use flashing LED.
     * If set to true it will use the default flashing pattern.
     * Available only on Android platform.
     */
    CheckBox* mFlash;

    /**
     * The flash pattern: the color, ledOnLength, ledOffLength.
     */
    EditBox* mFlashColor;
    EditBox* mFlashOnLength;
    EditBox* mFlashOffLength;

    /**
     * Used to get the number of seconds on which the notification will be fired.
     */
    EditBox* mTime;

    /**
     * On click will create and schedule the local notification.
     */
    ImageButton* mCreateNotificationButton;

    /**
     * Store local notification objects.
     */
    Vector<LocalNotification*> mLocalNotificationVector;
};

#endif /* CREATENOTIFICATIONSCREEN_H_ */
