/* Copyright (C) 2011 MoSync AB

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
 * @author Florin Leu
 * @date 15 Dec 2011
 **/

#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Layout.h>
#include <MAUI/Label.h>
#include <MAUI/WidgetSkin.h>
#include <MAUI/Image.h>
#include <ResCompiler/ResCompiler.h>

#include "MAHeaders.h"

#define PLATFORM_TEXT "Platform"
#define SCREEN_TEXT "Screen"

#define SKIN_X_START	16
#define SKIN_X_END		32
#define SKIN_Y_START	16
#define SKIN_Y_END		32

#define COLOR_BLACK		0x000000
#define COLOR_WHITE		0xFFFFFF

using namespace MAUtil;
using namespace MAUI;

/**
 * Moblet to be used as a template for a Resource Example application.
 */
class ResourceMoblet : public Moblet
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	ResourceMoblet()
	{
		createUI();

		//Show the main screen
		mMainScreen->show();
		bMainScreenVisible = true;
	}

	/**
	 * Destructor.
	 */
	~ResourceMoblet()
	{
		clean();
	}

	void getScreenSize()
	{
		/// Get screen dimensions.
		MAExtent size = maGetScrSize();

		/// Extract the screen width
		mScreenWidth = EXTENT_X(size);

		/// Extract the screen height
		mScreenHeight = EXTENT_Y(size);
	}

	/**
	 * Create the user interface.
	 */
	void createMainUI()
	{
		// Create a NativeUI screen that will hold layout and widgets.
		mMainScreen = new Screen();

		//Create a Vertical Layout that will hold widgets
		mMainLayout = new Layout(0, 0, mScreenWidth, mScreenHeight, NULL, 1, 2);
		mMainLayout->setBackgroundColor(COLOR_BLACK);
		mMainLayout->setDrawBackground(true);
		mMainLayout->setPaddingTop(4*mScreenHeight/10);

		// Create a font object.
		MAUI::Font* font = new MAUI::Font(RES_FONT);

		// Create a "skin" for the buttons. This will give the
		// buttons a customised appearance. The skin is divided
		// into nine parts ("nine patch image"), where the center
		// part will be stretched to fit the size of the button.
		WidgetSkin* buttonSkin = new MAUI::WidgetSkin(
			RES_BUTTON_PRESSED,   // Image for selected state.
			RES_BUTTON_IDLE, // Image for unselected state.
			SKIN_X_START, // X coordinate for start of center patch.
			SKIN_X_END, // X coordinate for end of center patch.
			SKIN_Y_START, // Y coordinate for start of center patch.
			SKIN_Y_END, // Y coordinate for end of center patch.
			true,  // Is selected image transparent?
			true); // Is unselected image transparent?

		//Create a Native UI button
		mPlatformButton = new Label(0, mScreenHeight/10, mScreenWidth, mScreenHeight/10, mMainLayout, PLATFORM_TEXT, 0, font);
		mPlatformButton->setSkin(buttonSkin);

		// Centre the button text horizontally and vertically.
		mPlatformButton->setHorizontalAlignment(Label::HA_CENTER);
		mPlatformButton->setVerticalAlignment(Label::VA_CENTER);

		//Create a Native UI button
		mScreenButton = new Label(0, mScreenHeight/10, mScreenWidth, mScreenHeight/10, mMainLayout, SCREEN_TEXT, 0, font);
		mScreenButton->setSkin(buttonSkin);

		// Centre the button text horizontally and vertically.
		mScreenButton->setHorizontalAlignment(Label::HA_CENTER);
		mScreenButton->setVerticalAlignment(Label::VA_CENTER);

		//Add the layout to the screen
		mMainScreen->setMain(mMainLayout);
	}

	void createPlatformUI()
	{
		mPlatformScreen = new Screen();

		mPlatformLayout = new Layout(0, 0, mScreenWidth, mScreenHeight, NULL, 1, 1);
		mPlatformLayout->setBackgroundColor(COLOR_WHITE);
		mPlatformLayout->setDrawBackground(true);

		loadResource(RES_PLATFORM);

		int imgSize = maGetImageSize(RES_PLATFORM);

		mPlatformLayout->setPaddingLeft((mScreenWidth - EXTENT_X(imgSize)) >> 1);
		mPlatformLayout->setPaddingTop((mScreenHeight - EXTENT_Y(imgSize)) >> 1);

		mPlatformImage = new Image(0, 0, 0, 0, mPlatformLayout, true, true, RES_PLATFORM);
		mPlatformImage->setBackgroundColor(0xFFFFFF);

		mPlatformScreen->setMain(mPlatformLayout);
	}

	void createScreenUI()
	{
		mScreenScreen = new Screen();

		mScreenLayout = new Layout(0, 0, mScreenWidth, mScreenHeight, NULL, 1, 1);
		mScreenLayout->setBackgroundColor(COLOR_WHITE);
		mScreenLayout->setDrawBackground(true);

		loadResource(RES_SCREEN_TYPE);

		int imgSize = maGetImageSize(RES_SCREEN_TYPE);

		mScreenLayout->setPaddingLeft((mScreenWidth - EXTENT_X(imgSize)) >> 1);
		mScreenLayout->setPaddingTop((mScreenHeight - EXTENT_Y(imgSize)) >> 1);

		mScreenImage = new Image(0, 0, 0, 0, mScreenLayout, true, true, RES_SCREEN_TYPE);
		mScreenImage->setBackgroundColor(0xFFFFFF);

		mScreenScreen->setMain(mScreenLayout);
	}

	void createUI()
	{
		getScreenSize();
		createMainUI();
		createPlatformUI();
		createScreenUI();
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			if (bMainScreenVisible)
			{
				// Call close to exit the application.
				clean();
				close();
			}
			else
			{
				bMainScreenVisible = true;
				mMainScreen->show();
			}
		}
	}

	/**
	* This method is called when there is an touch-down event.
	* @param p The point where the touch event occurred.
	*/
	void pointerPressEvent(MAPoint2d p)
	{
		if (bMainScreenVisible)
		{
			if (mPlatformButton->contains(p.x, p.y))
			{
				mPlatformScreen->show();
				bMainScreenVisible = false;
			}
			else if (mScreenButton->contains(p.x, p.y))
			{
				mScreenScreen->show();
				bMainScreenVisible = false;
			}
		}
		else
		{
			bMainScreenVisible = true;
			mMainScreen->show();
		}
	}

	/**
	 * Cleans everything
	 */
	void clean()
	{
		//Unload the resources
		unloadResource(RES_PLATFORM);
		unloadResource(RES_SCREEN_TYPE);

		//Delete the screens.
		//All the children will be deleted.
		delete mPlatformScreen;
		mPlatformScreen = NULL;

		delete mScreenScreen;
		mScreenScreen = NULL;

		delete mMainScreen;
		mMainScreen = NULL;
	}

private:
	int mScreenWidth;
	int mScreenHeight;

	bool bMainScreenVisible;

    Screen* mMainScreen;				//A MAUI screen
    Layout* mMainLayout;				//A MAUI layout
    Label* mPlatformButton;				//A MAUI label
    Label* mScreenButton;				//A MAUI label

    Screen* mPlatformScreen;			//A MAUI screen
    Layout* mPlatformLayout;			//A MAUI layout
    Image* mPlatformImage;				//A MAUI image

    Screen* mScreenScreen;				//A MAUI screen
    Layout* mScreenLayout;				//A MAUI layout
    Image* mScreenImage;				//A MAUI image
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new ResourceMoblet());
	return 0;
}
