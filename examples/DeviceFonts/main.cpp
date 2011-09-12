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
 * use fonts installed in your device
 */

#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <wchar.h>
#include <mavsprintf.h>
#include <mastring.h>
#include <MAUtil/String.h>
#include <ma.h>
#include <maheap.h>

using namespace MAUtil;

/**
 * A Moblet is a high-level class that defines the
 * behaviour of a MoSync program.
 */
class MyMoblet : public Moblet
{
public:
	/**
	 * Initialize the application in the constructor.
	 */
	MyMoblet()
	{
		//Calculate the font size
		MAExtent ex = maGetScrSize();
		mScreenWidth = EXTENT_X(ex);
		mScreenHeight = EXTENT_Y(ex);
		mFontSize = mScreenHeight / 40;

		//Here we load a default font, which is used for the first collumn
		mDefaultFont = maFontLoadDefault(FONT_TYPE_SANS_SERIF, 0, mFontSize);

		//This variable is used for scrolling the screen
		mCurrentPosition = 0;

		//The additional empty space between two fonts
		mFontSpacer = 5;

		//Buffer for holding a font name
		char buffer[64];

		//The number of fonts installed in the device
		mNumFonts = maFontGetCount();

		//A list of font handles
		mFonts = new MAHandle[mNumFonts];

		//A list of font names
		mFontNames = new String*[mNumFonts];

		for (int i = 0; i < mNumFonts; i++)
		{
			//Get the name of the font at position i
			if (maFontGetName(i, buffer, 64))
			{
				//Load a font with that name, and get it's handle
				mFonts[i] = maFontLoadWithName(buffer, mFontSize);
				mFontNames[i] = new String(buffer, 16);
			}
		}

		printFonts();
	}

	/**
	 * Called whenever the screen needs to be updated
	 */
	void printFonts()
	{
		maSetColor(0x000000); //Background color
		maFillRect(0, 0, mScreenWidth,mScreenHeight);
		maSetColor(0xFFFFFF); //Text color

		//Position for the first font
		int y = 10 - mCurrentPosition;
		int x = 10;

		for (int i = 0; i < mNumFonts; i++)
		{
			//Set the font system to the default font
			maFontSetCurrent(mDefaultFont);

			//And print the name of the font we will print next
			maDrawText(x, y, mFontNames[i]->c_str());

			//Set the font system to the i-th font
			maFontSetCurrent(mFonts[i]);

			//Print it with maDrawText
			maDrawText(x + (mFontSize * 10), y, "maDrawText");

			//And again with maDrawTextW
			maDrawTextW(x + (mFontSize * 20), y, L"maDrawTextW");

			y += (mFontSize + mFontSpacer);
		}

		maUpdateScreen();
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
	 * Called when a key is released.
	 */
	void keyReleaseEvent(int keyCode, int nativeCode)
	{
	}

	/**
	 * Called when the screen is touched.
	 */
	void pointerMoveEvent(MAPoint2d point)
	{
		//We find the distance that the pointed was moved
		//since the last call to pointerMoveEvent
		int d = mLastPointerPosition - point.y;
		mLastPointerPosition = point.y;

		//We update our current position in the font list
		mCurrentPosition += d;

		//We make sure that we are not out of bounds
		mCurrentPosition = (mCurrentPosition < 0)
								?0
								:mCurrentPosition;
		mCurrentPosition = (mCurrentPosition > (mNumFonts - 1) * (mFontSize + mFontSpacer))
								?(mNumFonts - 1) * (mFontSize + mFontSpacer)
								:mCurrentPosition;

		printFonts();
	}

	void pointerPressEvent(MAPoint2d point)
	{
		mLastPointerPosition=point.y;
	}
private:
	int mScreenWidth;			//The width of the device screen
	int mScreenHeight;			//The height of the device screen
	MAHandle *mFonts;			//A pointer to an array of font handles
	String **mFontNames;		//A pointer to an array of String pointers (the font names)
	int mNumFonts;				//The number of fonts installed in the device
	int mFontSize;				//The font size which will be used
	int mFontSpacer;			//Vertical space between two lines
	int mCurrentPosition;		//Current screen position for scrolling through the font list
	int mLastPointerPosition;	//Helped variable for scrolling
	MAHandle mDefaultFont;		//The default font, used here for printing font names
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
