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

//#include <NativeUI/Button.h>
//#include <NativeUI/ListView.h>
//#include <NativeUI/ListViewListener.h>
//#include <NativeUI/Screen.h>

#include "FacebookDemoScreen.h"
#include "Facebook/LOG.h"

#include "../PlatformInfo.h"

namespace FacebookDemoGUI
{

FacebookDemoScreen::FacebookDemoScreen(FacebookDemoScreen *prevScreen): mPreviousScreen(prevScreen),
		mLayout(0),
		mList(0),
		mBackButton(0)
{
	mClearScreenAfterLosingFocus = false;

	mItemsColor_Android = 0x000000;
	mItemsColor_iPhone = 0x99999A;

	mScreenColor = 0x000000;

	this->setBackgroundColor(mScreenColor);
}

void FacebookDemoScreen::show()
{
	if(mPreviousScreen)
	{
		mPreviousScreen->receiveKeyEvents(false);
	}

	receiveKeyEvents(true);
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

void FacebookDemoScreen::add(ListItem *btn)
{
	if(FacebookDemoApplication::isAndroid())
	{
		btn->setBackgroundColor(mItemsColor_Android);
	}
	else
	{
		btn->setBackgroundColor(mItemsColor_iPhone);
	}
	mList->addChild(btn);
}


void FacebookDemoScreen::clearScreenAfterLosingFocus(bool clearScreen)
{
	mClearScreenAfterLosingFocus = clearScreen;
}

bool FacebookDemoScreen::isEmpty() const
{
	return (mList->countChildWidgets() == 0);
}

int FacebookDemoScreen::addChild(NativeUI::Widget* widget)
{
	return NativeUI::Screen::addChild(widget);
}

FacebookDemoScreen::~FacebookDemoScreen()
{
	receiveKeyEvents(false);
}

void FacebookDemoScreen::back()
{
	if(0 != mPreviousScreen)
	{
		receiveKeyEvents(false);
		mPreviousScreen->show();
	}
}

}//namespace FacebookDemoGUI
