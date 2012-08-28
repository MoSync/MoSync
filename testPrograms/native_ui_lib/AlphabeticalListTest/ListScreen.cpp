/*
 Copyright (C) 2012 MoSync AB

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

/**
 * ListScreen.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: Spiridon Alexandru
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <NativeUI/ListViewSection.h>

#include "ListScreen.h"

/**
 * Constructor.
 */
ListScreen::ListScreen() :
	Screen(),
	mMainLayout(NULL),
	mListView(NULL)
{
	createMainLayout();

	mListView->addListViewListener(this);
}

/**
 * Destructor.
 */
ListScreen::~ListScreen()
{
	mListView->removeListViewListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void ListScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	mListView = new ListView(LIST_VIEW_TYPE_ALPHABETICAL);

	lprintfln("OK");

	ListViewItem* item1 = new ListViewItem();
//	item1->setText("firstItem");

	Button* newButton = new Button();
	newButton->setHeight(500);
	newButton->setWidth(100);
	newButton->setText("test");
	item1->addChild(newButton);

	ListViewItem* item2 = new ListViewItem();
//	item2->setText("secondItem");

	Button* newButton2 = new Button();
	newButton2->setHeight(300);
	newButton2->setWidth(200);
	newButton2->setText("test2");
	item2->addChild(newButton2);

	ListViewSection* newSection = new ListViewSection();
	newSection->setTitle("A");
	newSection->addItem(item1);
	newSection->addItem(item2);

	mListView->addChild(newSection);

	ListViewItem* item3 = new ListViewItem();
//	item3->setText("secondItem");

	Button* newButton3 = new Button();
	newButton3->setHeight(300);
	newButton3->setWidth(200);
	newButton3->setText("test3");
	item3->addChild(newButton3);

	ListViewSection* newSection2 = new ListViewSection();
	newSection2->setTitle("B");
	newSection2->addItem(item3);

	mListView->addChild(newSection2);

/*	this->setItemText(item1, "first item");
	this->setItemBackground(item1, 0xff0000);
	this->setItemIcon(item1, IMG);
	this->setItemAccessoryType(item1, LIST_VIEW_ITEM_ACCESSORY_CHECKMARK);
	this->setItemFontSize(item1, 30);
	this->setItemFontColor(item1, 0x00ff00);
	this->setItemFont(item1); */


//	mListView->addChild(item1);

	// The second item has different font.
/*	ListViewItem* item2 = new ListViewItem();
	item2->setText("secondItem");
	this->setItemText(item2, "first item");
	this->setItemBackground(item2, 0xffff00);
	this->setItemAccessoryType(item2, LIST_VIEW_ITEM_ACCESSORY_CHECKMARK);
	this->setItemFontSize(item2, 30);
	this->setItemFontColor(item2, 0xff0000);
	this->setItemFont(item2);
	item2->fillSpaceHorizontally();
	mListView->addChild(item2); */

	int result = mMainLayout->addChild(mListView);
    printf("add mListView result = %d", result);
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void ListScreen::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
    if (listView == mListView)
    {
        printf("mListView event: item clicked");
        listViewItem->setText("item clicked");
        listViewItem->setBackgroundColor(0x0000ff);
    }
}
