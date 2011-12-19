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
 * @brief Screen used for displaying push notification's values.
 */

#ifndef DISPLAYNOTIFICATIONSCREEN_H_
#define DISPLAYNOTIFICATIONSCREEN_H_

#define DEFAULT_SPACER_WIDTH 10

#include <NativeUI/Widgets.h>
#include <MAUtil/String.h>

using namespace NativeUI;
using namespace MAUtil;

// Forward declarations
namespace Notification
{
	class PushNotification;
}

/**
 * @brief Screen used for displaying push notification's values.
 */
class DisplayNotificationScreen:
	public Screen,
	public ButtonListener,
	public EditBoxListener
{
public:
	/**
	 * Constructor.
	 */
	DisplayNotificationScreen();

	/**
	 * Destructor.
	 */
	~DisplayNotificationScreen();

	/**
	 * Displays notification's content on the screen.
	 */
	void pushNotificationReceived(
		Notification::PushNotification& pushNotification);

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Helper function to create list view item with
	 * specific label and edit/check box.
	 */
	ListViewItem* createListViewItem(
		const MAUtil::String& labelText, Widget* widget);

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
     * Displays push notification's alert message.
     */
    Label* mMessageLabel;

    /**
     * Displays push notification's sound file name.
     * Only on iOS platform.
     */
    Label* mSoundFileNameLabel;

    /**
     * Displays push notification's icon badge number.
     * Only on iOS platform.
     */
    Label* mBadgeNumberLabel;
};

#endif /* DISPLAYNOTIFICATIONSCREEN_H_ */
