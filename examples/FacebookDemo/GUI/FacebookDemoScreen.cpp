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

/*
 * FacebookDemoScreen.cpp
 *
 *  Created on: Aug 8, 2011
 *      Author: gabi
 */

#include "FacebookDemoScreen.h"
#include <Facebook/LOG.h>

namespace FacebookDemoGUI
{

FacebookDemoScreen::FacebookDemoScreen(FacebookDemoScreen *prevScreen): mPreviousScreen(prevScreen), mBusy(false)
{
}

void FacebookDemoScreen::show()
{
	if(mPreviousScreen)
	{
		mPreviousScreen->receiveKeyEvents(false);
	}
	MoSync::UI::Screen::show();
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

void FacebookDemoScreen::FacebookDemoScreen::setBusy(bool busy)
{
	mBusy = busy;
}

bool FacebookDemoScreen::FacebookDemoScreen::isBusy() const
{
	return mBusy;
}

/**
 * Called when a key is pressed.
 */
void FacebookDemoScreen::keyPressEvent(int keyCode, int nativeCode)
{
	LOG("\t\t!FacebookDemoScreen::keyPressEvent keyCode=%d; mPrevScreen=%p, nativeCode=%d",
		keyCode, mPreviousScreen, nativeCode);
	if (MAK_BACK == keyCode )
	{

		receiveKeyEvents(false);
		if(0 != mPreviousScreen)
		{
			mPreviousScreen->show();
		}
	}
	if( MAK_HOME == keyCode)
	{
		maExit(0);
	}
}

FacebookDemoScreen *FacebookDemoScreen::getPreviousScreen()
{
	return mPreviousScreen;
}

FacebookDemoScreen::~FacebookDemoScreen()
{
	receiveKeyEvents(false);
//	LOG("\n\t\tFacebookDemoScreen DESTRUCTOR\n this=%d", this);
}

}//namespace FacebookDemoGUI
