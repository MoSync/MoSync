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
