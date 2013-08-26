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
