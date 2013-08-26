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

void ListScreen::listViewItemClicked(
	NativeUI::ListView *listView,
	NativeUI::ListViewSection *listViewSection,
	NativeUI::ListViewItem *listViewItem)
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
