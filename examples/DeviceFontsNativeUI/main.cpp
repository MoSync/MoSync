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
 * @file main.cpp
 * @author Iraklis Rossis
 *
 * This application provides a basic example of how to find and
 * use fonts installed in your device using Native UI widgets
 */

#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>

using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet to be used as a template for a Native UI application.
 */
class DeviceFontsNativeUI : public Moblet
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	DeviceFontsNativeUI()
	{
		createUI();
	}

	/**
	 * In the destructor, we destroy the widgets to release memory.
	 * Destroying a widget automatically destroys all child widgets.
	 */
	virtual ~DeviceFontsNativeUI()
	{
		// Destroying the screen widget destroys all of its children.
		delete mScreen;
	}

	/**
	 * Create the user interface.
	 */
	void createUI()
	{
		// Create a NativeUI screen that will hold layout and widgets.
		mScreen = new Screen();

		// Create a list view.
		mList = new ListView();

		mList->fillSpaceHorizontally();
		mList->wrapContentVertically();

		mScreen->setMainWidget(mList);

		//Set a font size. This is independent of screen size.
		int fontSize = 20;

		//Get the number of fonts installed in the device
		int mNumFonts = maFontGetCount();

		//Set a buffer for font names
		char buffer[64];

		for (int i = 0; i < mNumFonts; i++)
		{
			//Get the i-th font name
			if (maFontGetName(i, buffer, 64))
			{
				//Load the font, and get it's handle
				MAHandle font = maFontLoadWithName(buffer, fontSize);

				//Create a list item
				ListViewItem *listItem = new ListViewItem();

				listItem->fillSpaceHorizontally();
				listItem->wrapContentVertically();
				listItem->setText(buffer);
				listItem->setFont(font);

				mList->addChild(listItem);
			}
		}

		// Show the screen.
		mScreen->show();

		// Make the Moblet listen to widget events.
		Environment::getEnvironment().addCustomEventListener(this);
	}

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

	/**
	 * Method that implements the custom event listener interface.
	 * Widget events are sent as custom events.
	 */
	void customEvent(const MAEvent& event)
	{

	}

private:

	Screen *mScreen;

	ListView *mList;

};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new DeviceFontsNativeUI());
	return 0;
}
