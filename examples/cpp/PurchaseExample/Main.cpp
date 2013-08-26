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
