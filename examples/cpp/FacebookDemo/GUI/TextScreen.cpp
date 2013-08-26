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
 * TextScreen.cpp
 */

#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/Button.h>

#include "TextScreen.h"
#include "Facebook/LOG.h"

#include "../PlatformInfo.h"

namespace FacebookDemoGUI
{

TextScreen::TextScreen(FacebookDemoScreen *prev):FacebookDemoScreen(prev)
{
	initialize();
}

void TextScreen::setText(const MAUtil::String &text)
{
	mLabel->setText(text);
}

void TextScreen::clear()
{
	mLabel->setText("");
}

void TextScreen::buttonPressed(Widget* button)
{
}

void TextScreen::buttonReleased(Widget* button)
{
}

void TextScreen::buttonClicked(Widget* button)
{
	if( button = mBackButton )
	{
		back();
		mPreviousScreen->show();
	}
}

void TextScreen::initialize()
{
	mLayout = new NativeUI::VerticalLayout();

	mLabel = new NativeUI::Label();
	mLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mLabel->fillSpaceHorizontally();
	mLabel->fillSpaceVertically();

	if( FacebookDemoApplication::isAndroid())
	{
		mLayout->setBackgroundColor(0x000000);
		mLabel->setBackgroundColor(0x000000);
	}
	else
	{
		mLayout->setBackgroundColor(0x99999A);
		mLabel->setBackgroundColor(0x99999A);
	}

	mBackButton = new NativeUI::Button();
	mBackButton->setText("back");
	mBackButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mBackButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mBackButton->fillSpaceHorizontally();
	mBackButton->addButtonListener(this);

	mLayout->addChild(mLabel);
	mLayout->addChild(mBackButton);

	setMainWidget(mLayout);
}

}//namespace FacebookDemoGUI
