/*
 * FacebookDemoScreen.cpp
 *
 *  Created on: Aug 8, 2011
 *      Author: gabi
 */

#include "FacebookDemoScreen.h"
#include "Facebook/LOG.h"

namespace FacebookDemoGUI
{

FacebookDemoScreen::FacebookDemoScreen(FacebookDemoScreen *prevScreen): mPreviousScreen(prevScreen)
{
}

void FacebookDemoScreen::show()
{
	if(mPreviousScreen)
	{
		mPreviousScreen->receiveKeyEvents(false);
	}
	NativeUI::Screen::show();
}

void FacebookDemoScreen::receiveKeyEvents(bool receive)
{
	bool registered = MAUtil::Environment::getEnvironment().isKeyListener(this);

	if( receive )
	{
		if(!registered)
		{
			MAUtil::Environment::getEnvironment().addKeyListener(this);
		}
	}
	else
	{
		if(registered)
		{
			MAUtil::Environment::getEnvironment().removeKeyListener(this);
		}
	}
}

/**
 * Called when a key is pressed.
 */
void FacebookDemoScreen::keyPressEvent(int keyCode, int nativeCode)
{
	if (MAK_BACK == keyCode )
	{
		back();
	}
}

FacebookDemoScreen *FacebookDemoScreen::getPreviousScreen()
{
	return mPreviousScreen;
}

FacebookDemoScreen::~FacebookDemoScreen()
{
	receiveKeyEvents(false);
}

void FacebookDemoScreen::back()
{
	receiveKeyEvents(false);
	if(0 != mPreviousScreen)
	{
		mPreviousScreen->show();
	}
}

}//namespace FacebookDemoGUI
