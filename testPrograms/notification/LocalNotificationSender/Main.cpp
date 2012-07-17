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
 * @file Main.cpp
 * @author Bogdan Iusco
 *
 * This is the main entry point for the application
 */

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Environment.h>
#include <conprint.h>

#include <Notification/NotificationManager.h>

#include "CreateNotificationScreen.h"
#include "Util.h"

/**
 * Moblet for the  application.
 */
class NativeUIMoblet : public MAUtil::Moblet,
					   public MAUtil::FocusListener
{
public:
	/**
	 * Constructor that creates the UI.
	 */
	NativeUIMoblet()
	{
		if (!isAndroid() && !isIOS())
		{
			maMessageBox("Error", "Run this program on Android or iOS devices");
		}
		else
		{

			// Create the main user interface screen.
			mMainScreen = new CreateNotificationScreen();

			// Show the screen.
			mMainScreen->show();
		}

		this->addFocusListener(this);
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		this->removeFocusListener(this);
		delete mMainScreen;
	}

	/**
	 * Called when the application goes to background.
	 */
	virtual void focusLost()
	{
		// No implementation required.
	}

	/**
	 * Called when the application comes to foreground.
	 */
	virtual void focusGained()
	{
		NotificationManager::getInstance()->setApplicationIconBadgeNumber(0);
	}

	/**
	 * This method is called when the application is closed.
	 */
	void NativeUIMoblet::closeEvent()
	{
		// Deallocate the main screen.
		delete mMainScreen;
		mMainScreen = NULL;

		// Exit the app.
		close();
	}

	/**
	 * Method called when a key is pressed.
	 */
	void NativeUIMoblet::keyPressEvent(int keyCode, int nativeCode)
	{
	    // Close the application if the back key is pressed.
	    if(MAK_BACK == keyCode)
	    {
	        closeEvent();
	    }
	}
private:
	CreateNotificationScreen* mMainScreen;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create a moblet.
	NativeUIMoblet* moblet = new NativeUIMoblet();

	// Run the moblet event loop.
	MAUtil::Moblet::run(moblet);

	// Deallocate objects.
	delete moblet;

	return 0;
}
