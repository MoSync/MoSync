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
