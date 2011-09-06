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

ListScreen::ListScreen(FacebookDemoScreen *prev):FacebookDemoScreen(prev), mClearScreenAfterLosingFocus(false)
{
	LOG("\n\tListScreen(%d) constructor", prev);
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
	mList->addChild(btn);
}

void ListScreen::clear()
{
	LOG("\n\tListScreen::clear()");
	initialize();
}

void ListScreen::clearScreenAfterLosingFocus(bool clear)
{
	mClearScreenAfterLosingFocus = clear;
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
	NativeUI::Screen::addChild(widget);
}

void ListScreen::initialize()
{
	LOG("\n\tListScreen::initialize()");
	NativeUI::VerticalLayout *layout = new NativeUI::VerticalLayout();

	NativeUI::ListView *list = new NativeUI::ListView();
	list->addListViewListener(this);
	list->setBackgroundColor(0x000000);

	NativeUI::Button *button = new NativeUI::Button();
	button->addButtonListener(this);
	button->setBackgroundColor(0xF0F0F0);
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

	this->setBackgroundColor(0x000000);
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
