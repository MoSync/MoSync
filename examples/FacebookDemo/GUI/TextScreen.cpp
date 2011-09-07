/*
 * TextScreen.cpp
 *
 *  Created on: Aug 6, 2011
 *      Author: gabi
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

TextScreen::TextScreen(): FacebookDemoScreen(0)
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
	back();
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
