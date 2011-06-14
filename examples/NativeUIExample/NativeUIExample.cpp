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
 * @file NativeUIExample.cpp
 * @author Mikael Kindborg
 *
 * This is the main entry point for the example application
 * that demonstrates NativeUI on MoSync.
 */

#include <ma.h>
#include <MAUtil/Moblet.h>
#include "UIWrapper/Widgets.h"
#include "ScreenMain.h"

using namespace MAUtil;
using namespace MoSync::UI;

/**
 * Moblet for the  application.
 */
class NativeUIMoblet : public Moblet
{
public:
	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create a moblet.
	NativeUIMoblet* moblet = new NativeUIMoblet();

	// Create the user interface.
	Screen* mainScreen = ScreenMain::create();
	mainScreen->show();

	// Run the moblet event loop.
	Moblet::run(moblet);

	// TODO: Deallocate objects.

	return 0;
}
