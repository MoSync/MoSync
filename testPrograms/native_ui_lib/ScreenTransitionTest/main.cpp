/*
 Copyright (C) 2013 MoSync AB

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
				else
				{
					close();
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
