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
 * @author Mikael Kindborg
 *
 * This is the main entry point for the example application
 * that demonstrates NativeUI on MoSync.
 */

#include <MAUtil/Moblet.h>

#include "Controller/AppController.h"

namespace ImageTest
{

	/**
	 * Moblet for the  application.
	 */
	class NativeUIMoblet : public MAUtil::Moblet
	{
	public:
		/**
		 * Constructor that creates the UI.
		 */
		NativeUIMoblet()
		{
			mAppController = new AppController();
		}

		/**
		 * Destructor.
		 */
		virtual ~NativeUIMoblet()
		{
			delete mAppController;
		}

		/**
		 * This method is called when the application is closed.
		 */
		void NativeUIMoblet::closeEvent()
		{
			// Deallocate the main screen.
			delete mAppController;
			mAppController = NULL;

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
		AppController* mAppController;
	};

} // end of ImageTest

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create a moblet.
	ImageTest::NativeUIMoblet* moblet = new ImageTest::NativeUIMoblet();

	// Run the moblet event loop.
	MAUtil::Moblet::run(moblet);

	// Deallocate objects.
	delete moblet;

	return 0;
}
