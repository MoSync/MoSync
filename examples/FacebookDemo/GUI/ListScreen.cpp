/*
 * ListScreen.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#include "ListScreen.h"
#include "Facebook/LOG.h"

#include <NativeUI/Button.h>
#include <NativeUI/ListView.h>

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
	receiveKeyEvents(true);
}

void ListScreen::add(ListItem *btn)
{
	btn->setBackgroundColor(mListItemsColor);
	mList->addChild(btn);
}

void ListScreen::clear()
{
	LOG("\n\tListScreen::clear()");
	initialize();
}

void ListScreen::clearScreenAfterLosingFocus(bool clearScreen)
{
	mClearScreenAfterLosingFocus = clearScreen;
}

ListScreen::~ListScreen()
{
}

bool ListScreen::isEmpty() const
{
	return (mList->countChildWidgets() == 0);
}

int ListScreen::addChild(NativeUI::Widget* widget)
{
	return NativeUI::Screen::addChild(widget);
}

void ListScreen::initialize()
{
	LOG("\n\tListScreen::initialize()");
	mClearScreenAfterLosingFocus = false;
	mListItemsColor = 0x000088;
	mScreenColor = 0x000000;

	this->setBackgroundColor(mScreenColor);

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
