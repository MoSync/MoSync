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
 * @file SettingsScreen.h
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Screen used for configuring environment for receiving push notification.
 */

#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_

#define DEFAULT_SPACER_WIDTH 10

#include <NativeUI/Widgets.h>
#include <MAUtil/String.h>

using namespace NativeUI;
using namespace MAUtil;

// Forward declarations
class SettingsScreenListener;

/**
 * @brief Screen used for configuring environment for receiving push notification.
 */
class SettingsScreen:
	public Screen,
	public ButtonListener,
	public EditBoxListener,
	public CheckBoxListener
{
public:
	/**
	 * Constructor.
	 * @param listener Screen listener.
	 */
	SettingsScreen(SettingsScreenListener* listener);

	/**
	 * Destructor.
	 */
	virtual ~SettingsScreen();

	/**
	 * Sets connection status label's text to connected and hides the
	 * connect button.
	 */
	void connectionEstablished();

	/**
	 * Sets connection status label's text to not connected and enables the
	 * connect button.
	 */
	void connectionFailed();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Reset view's content.
	 * Set default values for widgets.
	 */
	void resetView();

	/**
	 * Checks if the user input data is valid(e.g. numeric edit box values
	 * can be converted to integer).
	 * @return True if the data is valid, false otherwise.
	 */
	bool isUserInputDataValid();

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
     void buttonClicked(Widget* button);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    void editBoxReturn(EditBox* editBox);

    /**
     * This method is called when the state of the check box was changed
     * by the user.
     * @param checkBox The check box object that generated the event.
     * @param state True if the check box is checked, false otherwise.
     */
    void checkBoxStateChanged(
        CheckBox* checkBox,
        bool state);

    /**
     * Creates an empty widget with a given width.
     * The ownership of the result is passed to the caller.
     * @return An empty widget.
     */
    Widget* createSpacer(const int width = DEFAULT_SPACER_WIDTH);

    /**
    * Helper function to create list view item with
    * specific label and edit/check box.
    */
    ListViewItem* createListViewItem(const MAUtil::String& labelText, Widget* widget);

private:

    /**
     * Listener for screen.
     */
    SettingsScreenListener* mListener;

    /**
     * Used for getting server's IP address.
     */
    EditBox* mIPEditBox;

    /**
     * Used for getting server's port number.
     */
    EditBox* mPortEditBox;

    /**
     * Used for displaying connection status.
     */
    Label* mConnectionStatusLabel;

    /**
     * Used for establish connection with server.
     */
    Button* mConnectButton;

    // Android only
    /**
     * The ticker text for incoming notifications.
     * ( The text that flows by in the status bar when the
     * notification first arrives.
     */
    EditBox* mTickerText;

    /**
     * The title in the content area.
     */
    EditBox* mContentTitle;

    /**
     * The incoming notifications will be displayed only if the app
     * is running in background.
     * Android only.
     */
    CheckBox* mShowOnlyIfInBackground;
};

#endif /* SETTINGSSCREEN_H_ */
