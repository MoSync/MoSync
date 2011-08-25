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
 * TextScreen.cpp
 *
 *  Created on: Aug 6, 2011
 *      Author: gabi
 */

#include "TextScreen.h"
#include <Facebook/LOG.h>

namespace FacebookDemoGUI
{

TextScreen::TextScreen(FacebookDemoScreen *prev):FacebookDemoScreen(prev)
{
	mLabel = new MoSync::UI::Label();
	initialize(mLabel);
	setMainWidget(mLabel);
}

TextScreen::TextScreen(): FacebookDemoScreen(0)
{
	mLabel = new MoSync::UI::Label();
	initialize(mLabel);
	setMainWidget(mLabel);
}

void TextScreen::setText(const MAUtil::String &text)
{
	mLabel->setText(text);
}

void TextScreen::clear()
{
	mLabel->setText("");
}

void TextScreen::addChild(MoSync::UI::Widget *widget)
{
	MoSync::UI::Screen::addChild(widget);
}

void TextScreen::initialize(MoSync::UI::Label *label)
{
	MAExtent screenSize = maGetScrSize();
	int scrHeight = EXTENT_Y(screenSize);
	label->setFontSize(scrHeight/16);

	label->setProperty(MAW_LABEL_TEXT_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);
	label->setProperty(MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_LEFT);
}

}//namespace FacebookDemoGUI
