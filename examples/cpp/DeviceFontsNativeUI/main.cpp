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
