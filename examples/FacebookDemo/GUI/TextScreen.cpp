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
 * TextScreen.cpp
 */

#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/Button.h>

#include "TextScreen.h"
#include "Facebook/LOG.h"

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
