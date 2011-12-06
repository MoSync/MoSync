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

/**
 * @file MainScreen.cpp
 * @author Bogdan Iusco.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <maxtoa.h>

#include "MainScreen.h"
#include "MAHeaders.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mListView(NULL),
	mAddItemButton(NULL),
	mRemoveItemButton(NULL)
{
	createMainLayout();

	mAddItemButton->addButtonListener(this);
	mRemoveItemButton->addButtonListener(this);
	mListView->addListViewListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mAddItemButton->removeButtonListener(NULL);
    mRemoveItemButton->removeButtonListener(NULL);
    mListView->removeListViewListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x99CCFF);
	Screen::setMainWidget(mMainLayout);

	mListView = new ListView();

	ListViewItem* item1 = new ListViewItem();
	this->setItemText(item1, "first item");
	this->setItemBackground(item1, 0xff0000);
	this->setItemIcon(item1, IMG);
	this->setItemAccessoryType(item1, LIST_VIEW_ITEM_ACCESSORY_CHECKMARK);
	this->setItemFontSize(item1, 30);
	this->setItemFontColor(item1, 0x00ff00);
	this->setItemFont(item1);


	mListView->addChild(item1);

	// The second item has different font.
	ListViewItem* item2 = new ListViewItem();
	this->setItemText(item2, "first item");
	this->setItemBackground(item2, 0xffff00);
	this->setItemAccessoryType(item2, LIST_VIEW_ITEM_ACCESSORY_CHECKMARK);
	this->setItemFontSize(item2, 30);
	this->setItemFontColor(item2, 0xff0000);
	this->setItemFont(item2);
	item2->fillSpaceHorizontally();
	mListView->addChild(item2);

	int result = mMainLayout->addChild(mListView);
    printf("add mListView result = %d", result);

	mAddItemButton = new Button();
	mAddItemButton->setText("Add item");
	mMainLayout->addChild(mAddItemButton);

	mRemoveItemButton = new Button();
	mRemoveItemButton->setText("Remove item");
	mMainLayout->addChild(mRemoveItemButton);
}

void MainScreen::setItemText(ListViewItem *item, const MAUtil::String &text)
{
	item->setText(text);
	printf("setText called");
}
void MainScreen::setItemBackground(ListViewItem *item, int color)
{
	item->setBackgroundColor(color);
	printf("setBackgroundColor called");
}
void MainScreen::setItemIcon(ListViewItem *item, MAHandle image)
{
	item->setIcon(image);
	printf("setIcon called");
}
void MainScreen::setItemAccessoryType(ListViewItem *item, NativeUI::ListViewItemAccessory accessory)
{
	item->setAccessoryType(accessory);
	printf("setAccessoryType called");
}
void MainScreen::setItemFontSize(ListViewItem *item, int size)
{
	item->setFontSize(size);
	printf("setFontSize called");
}
void MainScreen::setItemFontColor(ListViewItem *item, int color)
{
	item->setFontColor(color);
	printf("setFontColor called");
}

void MainScreen::setItemFont(ListViewItem *item)
{
//	int nrFonts = maFontGetCount();
//	// Get the first font
//	char buf[256];
//	maFontGetName(0, buf, 256);
//	// Load the font with size 15 and get a handle.
//	int fontHandle = maFontLoadWithName(buf, 15);

	//item->setFont(fontHandle);
	printf("setFont called");
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void MainScreen::listViewItemClicked(
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

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mAddItemButton)
    {
        ListViewItem* item = new ListViewItem();
        char buf[256];
        itoa(mVector.size(), buf,10);
        item->setText(buf);
        mVector.add(item);
        int result = mListView->addChild(item);
        printf("result addChild = %d", result);
    }
    else if (button == mRemoveItemButton)
    {
        if (mVector.size() > 0)
        {
            Widget* item = mVector[mVector.size() - 1];
            mVector.remove(mVector.size() -1);
            mListView->removeChild(item);
        }
    }
}
