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
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Screen used for configuring environment for receiving push notification.
 */

#define IP_LABEL_TEXT "IP:"
#define PORT_LABEL_TEXT "Port:"
#define CONNECTION_NOT_ESTABLISHED "Connection status: not connected!"
#define CONNECTION_CONNECTING "Connection status: connecting.."
#define CONNECTION_ESTABLISHED "Connection status: connected!"

#define CONNECT_BUTTON_TEXT "Connect"

// Default width for numeric edit box widgets.
#define NUMERIC_EDIT_BOX_WIDTH 70

// Used for verifying if a string can be converted to integer.
#define NINE 9
#define ZERO 0

// Text colors
#define TEXT_COLOR 0x000000
#define TEXT_COLOR_ERROR 0xFF0000

#define SCREEN_TITLE_NAME "Settings"

#include <maapi.h>
#include <MAUtil/util.h>

#include "SettingsScreen.h"
#include "Util.h"
#include "TCPConnection.h"
#include "Listeners.h"

/**
 * Constructor.
 * @param listener Screen listener.
 */
SettingsScreen::SettingsScreen(SettingsScreenListener* listener):
	Screen(),
	mListener(listener),
	mIPEditBox(NULL),
	mPortEditBox(NULL),
	mConnectionStatusLabel(NULL),
	mConnectButton(NULL)
{
	setTitle(SCREEN_TITLE_NAME);
	this->createMainLayout();

	mIPEditBox->addEditBoxListener(this);
	mPortEditBox->addEditBoxListener(this);
	mConnectButton->addButtonListener(this);

	mIPEditBox->setText("192.168.1.116");
	mPortEditBox->setText("4567");
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	mIPEditBox->removeEditBoxListener(this);
	mPortEditBox->removeEditBoxListener(this);
	mConnectButton->removeButtonListener(this);
}

/**
 * Sets connection status label's text to connected and hides the
 * connect button.
 */
void SettingsScreen::connectionEstablished()
{
	mConnectionStatusLabel->setText(CONNECTION_ESTABLISHED);
	mConnectButton->setVisible(false);
}

/**
 * Sets connection status label's text to not connected and enables the
 * connect button.
 */
void SettingsScreen::connectionFailed()
{
	mConnectionStatusLabel->setText(CONNECTION_NOT_ESTABLISHED);
	mConnectButton->setEnabled(true);
}

/**
 * Creates and adds main layout to the screen.
 */
void SettingsScreen::createMainLayout()
{

	VerticalLayout* mainLayout = new VerticalLayout();
	Screen::setMainWidget(mainLayout);

	ListView* listView = new ListView();
	listView->fillSpaceHorizontally();
	listView->fillSpaceVertically();
	mainLayout->addChild(listView);

	ListViewItem* listItem;

	// Add IP label and edit box
	mIPEditBox = new EditBox();
	mIPEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	listView->addChild(this->createListViewItem(IP_LABEL_TEXT, mIPEditBox));

	// Add port label and edit box
	mPortEditBox = new EditBox();
	mPortEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	listView->addChild(this->createListViewItem(PORT_LABEL_TEXT, mPortEditBox));

	// Add connection status label.
	listItem = new ListViewItem;
	listView->addChild(listItem);
	mConnectionStatusLabel = new Label();
	mConnectionStatusLabel->setText(CONNECTION_NOT_ESTABLISHED);
	listItem->addChild(mConnectionStatusLabel);

	listItem = new ListViewItem;
	listView->addChild(listItem);
	mConnectButton = new Button();
	mConnectButton->setText(CONNECT_BUTTON_TEXT);
	listItem->addChild(mConnectButton);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void SettingsScreen::buttonClicked(Widget* button)
{
	printf("SettingsScreen::buttonClicked");
	if (button == mConnectButton)
	{
		if (mListener)
		{
			mListener->connectToServer(mIPEditBox->getText(),
				mPortEditBox->getText());
		}
		mConnectButton->setEnabled(false);
		mConnectionStatusLabel->setText(CONNECTION_CONNECTING);
	}
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void SettingsScreen::editBoxReturn(EditBox* editBox)
{
	editBox->hideKeyboard();
}

/**
 * Reset view's content.
 * Set default values for widgets.
 */
void SettingsScreen::resetView()
{
}

/**
 * Checks if the user input data is valid(e.g. numeric edit box values
 * can be converted to integer).
 * @return True if the data is valid, false otherwise.
 */
bool SettingsScreen::isUserInputDataValid()
{
}

/**
 * Creates an empty widget with a given width.
 * The ownership of the result is passed to the caller.
 * @return An empty widget.
 */
Widget* SettingsScreen::createSpacer(const int width)
{
	HorizontalLayout* spacer = new HorizontalLayout();
	spacer->setWidth(width);
	return spacer;
}

/**
 * Helper function to create list view item with
 * specific label and edit box.
 */
ListViewItem* SettingsScreen::createListViewItem(
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
