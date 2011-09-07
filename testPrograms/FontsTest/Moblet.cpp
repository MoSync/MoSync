/*
 * Moblet.cpp
 *
 *  Created on: May 11, 2011
 *      Author: emma tresanszki
 */

#include <maxtoa.h>
#include <maapi.h>
#include <MAUtil/util.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

// The header file of the class.
#include "Moblet.h"

namespace Test
{

TestMoblet *TestMoblet::mInstance = NULL;

/*
 * ctor
 */
TestMoblet::TestMoblet()
{
	mShowing = 1;

	maSetColor(0xC1FFC1);

	int top = 10;
	maDrawText(10,top, "--------- Display number of fonts with current font ---------");
	nrFonts = maFontGetCount();
	MAUtil::String tempNrFonts;
	char buf1[10];
	char buf2[256];
	itoa(nrFonts,buf1, 10);
	tempNrFonts += buf1;
	tempNrFonts += " fonts.";
	top += 20;
	maDrawText(10,top,tempNrFonts.c_str());

	maSetColor(0x00FFFF);
	top+=40;
	maDrawText(10,top,"--------- Display list of fonts with default font size --------- ");
	MAUtil::String tempFont;
	int getNameError;

	for ( int i=0; i<nrFonts; i++)
	{
		char buf[256];
		getNameError = maFontGetName(i, buf, 256);
		if ( getNameError >= 0 )
		{
			top += 20;
			fontNames.add(buf);
			maDrawText(10,top, buf);
		}
		else
		{
			top += 20;
			message = "Font on index:" + MAUtil::integerToString(i) + "returned error:" + MAUtil::integerToString(getNameError);
			maDrawText(10, top, message.c_str());
		}
	}

	maDrawText(10, top+40, "Tap screen to test maLoadWithName for each font");
	maUpdateScreen();
}

void TestMoblet::testLoadWithName()
{
	mShowing = 2;

	maSetColor(0x000000);
	MAExtent size = maGetScrSize();
	maFillRect(0,0,EXTENT_X(size),EXTENT_Y(size) );

	maSetColor(0xCD5555);

	int top = 0;
	maDrawText(10,top,"--------- Test maFontLoadWithName(test each name from list) --------- ");
	top += 20;

	for (int i=0; i<fontNames.size(); i++)
	{
		top += 20;
		int handle = maFontLoadWithName(fontNames[i].c_str(), 15);
		if ( handle == RES_FONT_NAME_NONEXISTENT )
		{
			message = "Font nonexistent: " + fontNames[i];
			maDrawText(10,top, message.c_str());
		}
		else
		{
			maFontSetCurrent(handle);
			message =  "Font " + fontNames[i] + " has handle = " + MAUtil::integerToString(handle);
			maDrawText(10, top, message.c_str());
		}
	}

	maDrawText(10, top+40, "Tap screen to test maFontLoadDefault for some combinations");
	maUpdateScreen();
}

void TestMoblet::testLoadDefault()
{
	mShowing = 3;

	maSetColor(0x000000);
	MAExtent size = maGetScrSize();
	maFillRect(0,0,EXTENT_X(size),EXTENT_Y(size) );

	maSetColor(0xCD5555);

	int top = 0;
	maDrawText(10,top,"--------- Test maFontLoadDefault ---------");

	top += 20;
	maDrawText(10,top,"Test Monospace Normal -10");
	testFont(FONT_TYPE_MONOSPACE,FONT_STYLE_NORMAL,-10,top);

	top += 20;
	maDrawText(10, top, "Test Monospace Bold 15");
	testFont(FONT_TYPE_MONOSPACE, FONT_STYLE_BOLD, 15, top);

	top += 20;
	maDrawText(10, top, "Test Monospace Italic 15");
	testFont(FONT_TYPE_MONOSPACE, FONT_STYLE_ITALIC, 15, top);

	top += 20;
	maDrawText(10, top, "Test SansSerif Normal 15");
	testFont(FONT_TYPE_SANS_SERIF, FONT_STYLE_NORMAL, 15, top);

	top += 20;
	maDrawText(10, top, "Test SansSerif Bold 15");
	testFont(FONT_TYPE_SANS_SERIF, FONT_STYLE_BOLD, 15, top);

	top += 20;
	maDrawText(10, top, "Test SansSerif Italic 15");
	testFont(FONT_TYPE_SANS_SERIF, FONT_STYLE_ITALIC, 15, top);

	top += 20;
	maDrawText(10, top, "Test Serif Normal 15");
	testFont(FONT_TYPE_SERIF, FONT_STYLE_NORMAL, 15, top);

	top += 20;
	maDrawText(10, top, "Test Serif Bold 15");
	testFont(FONT_TYPE_SERIF, FONT_STYLE_BOLD, 15, top);

	top += 20;
	maDrawText(10, top, "Test Serif Italic 15");
	testFont(FONT_TYPE_SERIF, FONT_STYLE_ITALIC, 15, top);

	maDrawText(10, top+40, "Tap screen to EXIT");
	maUpdateScreen();
}

void TestMoblet::checkFont(int left, int top,MAHandle font)
{
	if ( font == -1)
	{
		maDrawText(200,top,"NO FONT!!");
	}

}

void TestMoblet::testFont (int family, int style, int size, int top)
{
	MAHandle font = maFontLoadDefault(family,style,size);
	if ( font == RES_FONT_INVALID_SIZE)
	{
		maSetColor(0xCD3333);
		maDrawText(200,top,"Invalid size");
		maSetColor(0xC1FFC1);
	}
	else if( font == RES_FONT_NO_TYPE_STYLE_COMBINATION)
	{
		maSetColor(0xCD3333);
		maDrawText(200,top,"No type style combination");
		maSetColor(0xC1FFC1);
	}
	else
	{
		maFontSetCurrent(font);
		MAUtil::String message = "Ok, handle: " + MAUtil::integerToString(font);
		maDrawText(200, top, message.c_str());
	}
}

/**
 * Get the single instance of this Moblet.
 * @return The Moblet instance.
 */
TestMoblet *TestMoblet::getInstance()
{
	if( NULL == mInstance ) {
		mInstance = new TestMoblet();
	}

	return mInstance;
}

/**
 * This method is called when the application is closed.
 */
void TestMoblet::closeEvent()
{
	// Exit the app.
	close();
}

/**
 * Method called when a key is pressed.
 */
void TestMoblet::keyPressEvent(int keyCode, int nativeCode)
{
    // Close the application if the back key is pressed.
    if(MAK_BACK == keyCode)
    {
        closeEvent();
    }
}

void TestMoblet::pointerPressEvent(MAPoint2d p)
{
	if ( mShowing == 1)
	{
		testLoadWithName();
	}
	else if ( mShowing == 2)
	{
		testLoadDefault();
	}
	else if ( mShowing == 3)
	{
		closeEvent();
	}
}

} // namespace Test
