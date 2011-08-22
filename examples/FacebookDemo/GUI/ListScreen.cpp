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
 * ListScreen.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#include "ListScreen.h"
#include <Facebook/LOG.h>

namespace FacebookDemoGUI
{

ListScreen::ListScreen(FacebookDemoScreen *prev):FacebookDemoScreen(prev),
		mBackgroundColor(0x000000), mTransparency(1.0) //1.0 fully visible, 0.5 - half visible, 0.0 - not visible
{
	initialize();
}

ListScreen::ListScreen():FacebookDemoScreen(0)
{
	initialize();
}

void ListScreen::show()
{
	FacebookDemoScreen::show();
	receiveKeyEvents(true);
}

void ListScreen::add(ListItem *btn)
{
	mList->addChild(btn);
}

void  ListScreen::remove(ListItem *btn)
{
	mList->remove(btn);
}

void ListScreen::clear()
{
	delete mList;
	initialize();
}

ListScreen::~ListScreen()
{
}

void ListScreen::setBackgroundColor(int color)
{
	mBackgroundColor = color;
	mList->setBackgroundColor(color);
}

bool ListScreen::isEmpty() const
{
	return mList->isEmpty();
}

//0.0 - not visible
//0.5 - half visible
//1.0 - fully visible
//1
void ListScreen::setTransparency(float alpha)
{
	mTransparency = alpha;
	mList->setProperty(MAW_WIDGET_ALPHA, alpha);
}

void ListScreen::addChild(MoSync::UI::Widget *widget)
{
	MoSync::UI::Screen::addChild(widget);
}

void ListScreen::initialize()
{
	mList = new MoSync::UI::ListView();
	mList->setBackgroundColor(mBackgroundColor);
	setProperty(MAW_WIDGET_ALPHA, mTransparency);
	setMainWidget(mList);
}

}//namespace FacebookDemoGUI
