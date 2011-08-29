/*
 * Moblet.cpp
 *
 *  Created on: May 11, 2011
 *      Author: emma tresanszki
 */

#include <MAUtil/String.h>

// The header file of the class.
#include "Moblet.h"
#include "maapi.h"
#include <maxtoa.h>

namespace Test
{

TestMoblet *TestMoblet::mInstance = NULL;

/*
 * ctor
 */
TestMoblet::TestMoblet():mMainScreen(NULL)
{
	mMainScreen = new NativeScreen();
	mMainScreen->showScreen();

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
	// Deallocate the main screen.
	delete mMainScreen;
	mMainScreen = NULL;

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

} // namespace Test
