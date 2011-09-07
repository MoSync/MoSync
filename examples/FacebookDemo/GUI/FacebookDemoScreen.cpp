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
 * FacebookDemoScreen.cpp
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
