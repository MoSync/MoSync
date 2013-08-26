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
 * @file main.cpp
 * @author Bogdan Iusco
 * @date 20 Nov 2012
 *
 * @brief App main entry point.
 */

#include <MAUtil/Moblet.h>

#include "Controller/Controller.h"
#include "Screens/ScreenUtils.h"

namespace ScreenTransitionTest
{
	class NativeUIMoblet : public MAUtil::Moblet
	{
	public:
		/**
		 * The constructor creates the user interface.
		 */
		NativeUIMoblet()
		{
			mController = new Controller();
		}

		/**
		 * Destructor.
		 */
		virtual ~NativeUIMoblet()
		{
			delete mController;
		}

		/**
		 * Called when a key is pressed.
		 */
		void keyPressEvent(int keyCode, int nativeCode)
		{
			if (MAK_BACK == keyCode || MAK_0 == keyCode)
			{
				// Call close to exit the application.
				/*
				 * Android runtime sends back key event before stack screen pop.
				 * This has been noted and soon the Android runtime will automatically
				 * pop the screen from stack when back button is pressed and send a pop
				 * event instead without a key event (only for non-root screens).
				 */
				if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
				{
					mController->handleAndroidBackButton();
				}
			}
		}

	private:
		Controller* mController;
	};

} // end of ScreenTransitionTest

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	MAUtil::Moblet::run(new ScreenTransitionTest::NativeUIMoblet());
	return 0;
}
