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
 * ListScreen.cpp
 */

#include "ListScreen.h"
#include "Facebook/LOG.h"
//#include <NativeUI/Button.h>
//#include <NativeUI/ListView.h>

#include "../PlatformInfo.h"

namespace FacebookDemoGUI
{

ListScreen::ListScreen(FacebookDemoScreen *prev): FacebookDemoScreen(prev)
{
	initialize();
}

ListScreen::ListScreen():FacebookDemoScreen(0)
{
	LOG("\n\tListScreen(0) constructor");
	initialize();
}

void ListScreen::show()
{
	FacebookDemoScreen::show();
}

void ListScreen::clear()
{
	LOG("\n\tListScreen::clear()");
	this->initialize();
}

ListScreen::~ListScreen()
{
}

void ListScreen::initialize()
{
	mClearScreenAfterLosingFocus = false;

	NativeUI::VerticalLayout *layout = new NativeUI::VerticalLayout();
	layout->setBackgroundColor(mScreenColor);

	NativeUI::ListView *list = new NativeUI::ListView();
	list->addListViewListener(this);
	list->setBackgroundColor(mScreenColor);

	NativeUI::Button *button = new NativeUI::Button();
	button->addButtonListener(this);
	button->setText("back");
	button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	button->fillSpaceHorizontally();

	layout->addChild(list);
	layout->addChild(button);

	setMainWidget(layout);

	mLayout = layout;
	mList = list;
	mBackButton = button;
}

void ListScreen::back()
{
	LOG("\n\tListScreen::back()");
	if( mClearScreenAfterLosingFocus )
	{
		clear();
	}
	FacebookDemoScreen::back();
}

void ListScreen::listViewItemClicked(NativeUI::ListView* listView, NativeUI::ListViewItem* listViewItem)
{
	ListItem *item = (ListItem*)listViewItem;
	item->doClick();
}

void ListScreen::buttonClicked(Widget* button)
{
	if(button == mBackButton)
	{
		back();
	}
}

}//namespace FacebookDemoGUI
