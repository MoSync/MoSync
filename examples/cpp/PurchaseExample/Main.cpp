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
 *
 * This is the main entry point for the example application
 * that demonstrates Purchase API on MoSync. (Available on Android and iOS)
 *
 * It is a basic example that demonstrates how to purchase
 * a product and how to get the receipt for it.
 * @author Emma Tresanszki
 */

#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class

#include <NativeUI/Widget.h>
#include <NativeUI/Widgets.h>// Include all widgets
#include <Purchase/PurchaseManager.h>

#include "Logic/Util.h"
#include "Logic/ApplicationController.h"
#include "Logic/Config.h"

using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet for the  application.
 */
class NativeUIMoblet : public Moblet
{
public:
	/**
	 * Constructor that creates the UI.
	 * If billing is not supported, no screen is shown.
	 */
	NativeUIMoblet():
		mController(NULL)
	{
		MAUtil::String developerKey = DEVELOPER_PUBLIC_KEY;
		int platform = getPlatform();
		int result = IAP::PurchaseManager::getInstance()->checkPurchaseSupported();
		if (platform != IOS &&
			platform != ANDROID)
		{
			maAlert("Error", "This program runs only on Android and iOS devices",
				"OK", NULL, NULL);
		}
		else if ( result != MA_PURCHASE_RES_OK )
		{
			MAUtil::String errorMessage = "Billing is not supported on this device, because of error "
					+ MAUtil::integerToString(result);
			maAlert("Error", errorMessage.c_str(), "OK", NULL, NULL);
		}
		else
		{
			if ( platform == ANDROID && developerKey.size() == 0 )
			{
				maAlert("Error", "You need to set developer key in Config.h ",
					"OK", NULL, NULL);
			}
			else if ( platform == ANDROID &&
					PurchaseManager::getInstance()->setPublicKey(DEVELOPER_PUBLIC_KEY)
						== MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY )
			{
				maAlert("Error", "Malformed developer key in Config.h ",
					"OK", NULL, NULL);
			}
			else
			{
				mController = new ApplicationController();
			}
		}
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		if (mController)
			delete mController;
	}
	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent()
	{

		// Exit the app.
		close();
	}

	/**
	 * Method called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
	    // Close the application if the back key is pressed.
	    if(MAK_BACK == keyCode)
	    {
	        closeEvent();
	    }
	}

private:
	/**
	 * App's controller.
	 */
	ApplicationController* mController;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create a moblet.
	NativeUIMoblet* moblet = new NativeUIMoblet();

	// Run the moblet event loop.
	Moblet::run(moblet);

	// Deallocate objects.
	delete moblet;

	return 0;
}
