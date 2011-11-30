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
	public EditBoxListener
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
};

#endif /* SETTINGSSCREEN_H_ */
