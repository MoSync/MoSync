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

/*
* @file main.cpp
*
*  Application that detects a change of screen orientation.
*  You should see a message that states the device orientation
*  and display size. When the device is rotated (F4 in the MoSync
*  emulator) the text will update to show the current orientation.
*
*  Press the zero or back key to exit on devices that support
*  key pads.
*
* @author Miles Midgley, Chris Hughes, Mikael Kindborg,
*/

#include <MAUtil/Moblet.h>
#include <conprint.h>

class MyMoblet : public MAUtil::Moblet
{
public:
	/**
	 * Constructor.
	 */
	MyMoblet()
	{
		// Set the orientation mode to dynamic, so that it is possible to switch
		// between portrait and landscape orientations. You can also use these
		// syscalls to lock the screen in PORTRAIT or LANDSCAPE mode. Not all
		// platforms support or need these syscalls.

		// Android and Windows Phone.
		maScreenSetOrientation(SCREEN_ORIENTATION_DYNAMIC);

		// iOS and Windows Phone.
		maScreenSetSupportedOrientations(
			MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT |
			MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT |
			MA_SCREEN_ORIENTATION_PORTRAIT |
			MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);

		// Update the display.
		drawScreen();
	}

	/**
	 * Method for handling key presses.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_0 == keyCode || MAK_BACK == keyCode)
		{
			maExit(0);
		}
	}

	/**
	 * Method for handling events that do not have predefined methods.
	 */
	void customEvent(const MAEvent& event)
	{
		// If the event type is screen changed we update the display.
		if (EVENT_TYPE_SCREEN_CHANGED == event.type)
		{
			drawScreen();
		}
	}

	/**
	 * Method that draws display data to the screen.
	 */
	void drawScreen()
	{
		MAExtent screenSize = maGetScrSize();
		int width = EXTENT_X(screenSize);
		int height = EXTENT_Y(screenSize);
		int x = 20;
		int y = height / 2;
		char orientationText[128];

		if (width > height) // Landscape
		{
			// Set the background color.
			maSetColor(0x000099);

			// Set text.
			sprintf(orientationText, "Landscape %d %d", width, height);
		}
		else // Portrait
		{
			// Set the background color.
			maSetColor(0x009900);

			// Set text.
			sprintf(orientationText, "Portrait %d %d", width, height);
		}

		// Fill background
		maFillRect(0, 0, width, height);

		// Use white to display the text.
		maSetColor(0xFFFFFF);

		// Draw the text.
		maDrawText(x, y, orientationText);

		// Redraw the screen.
		maUpdateScreen();
	}
};

// Entry point for the application.
extern "C" int MAMain()
{
	MAUtil::Moblet::run(new MyMoblet());
	return 0;
};
