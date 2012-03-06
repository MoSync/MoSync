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
 * @file ThirdScreen.cpp
 * @author emma
 *
 */

#include "ThirdScreen.h"

/**
 * Constructor.
 */
ThirdScreen::ThirdScreen():
	StackScreen()
{
	// Create the top screen in the stack.
	mScreen = new Screen();
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x104E8B);
	this->setTitle("Third");

	Label* info = new Label();
	info->setText("The third screen. It is a stack screen.2 Menu items, no icons.");
	mMainLayout->addChild(info);

	// Create the list view widget.
	mListView = new ListView();

	// Add items to the list view.
	for (int i = 0; i < 2; i++)
	{
		ListViewItem* colorItem = new ListViewItem();
		colorItem->setText("Push Next screen. On index" + MAUtil::integerToString(i));
		colorItem->setFontColor(0xFF0000);
		mListViewItems.add(colorItem);
		mListView->addChild(colorItem);
	}

	// Set event listener.
	mListView->addListViewListener(this);
	mMainLayout->addChild(mListView);

	// Set the list view as the main widget of the screen.
	mScreen->setMainWidget(mMainLayout);

	// Push the top screen.
	push(mScreen);

	mScreen->addOptionsMenuItem("Third screen item 0", -1,false);
	mScreen->addOptionsMenuItem("Third screen item 1", -1, false);
	mScreen->addScreenListener(this);

	// Set event listener.
	mListView->addListViewListener(this);
}

/**
 * Destructor.
 */
ThirdScreen::~ThirdScreen()
{
	mListView->removeListViewListener(this);
	this->removeScreenListener(this);
	if ( scr1 != NULL )
		scr1->removeScreenListener(this);
	if ( scr2 != NULL )
		scr2->removeScreenListener(this);
}

/**
 * This method is called when an item is clicked in the list widget.
 * @param listItemIndex The index of the list item that was clicked.
 */
void ThirdScreen::openNextScreen(int listItemIndex)
{
//	Screen* screen = new Screen();
	if ( listItemIndex == 0)
	{
		scr1 = new Screen();
		VerticalLayout* widget = new VerticalLayout();
		widget->setBackgroundColor(0x104E8B);
		Label* info = new Label();
		info->setText("First screen in stack.Menu items have icons(delete and save).");
		widget->addChild(info);
		mListView1 = new ListView();
		mListView1->fillSpaceHorizontally();
		mListView1->fillSpaceVertically();
		widget->addChild(mListView1);
		scr1->setMainWidget(widget);
		this->push(scr1);
		scr1->addScreenListener(this);
		scr1->addOptionsMenuItem("Menu item 0", MAW_OPTIONS_MENU_ICON_CONSTANT_DELETE,true);
		scr1->addOptionsMenuItem("Menu item 1", MAW_OPTIONS_MENU_ICON_CONSTANT_SAVE,true);

	}
	else
	{
		scr2 = new Screen();
		VerticalLayout* widget = new VerticalLayout();
		widget->setBackgroundColor(0x104E8B);
		Label* info = new Label();
		info->setText("Second screen in stack.Menu items have icons(edit and add)");
		widget->addChild(info);
		mListView2 = new ListView();
		mListView2->fillSpaceHorizontally();
		mListView2->fillSpaceVertically();
		widget->addChild(mListView2);
		scr2->setMainWidget(widget);
		this->push(scr2);
		scr2->addScreenListener(this);
		scr2->addOptionsMenuItem("Menu item 0", MAW_OPTIONS_MENU_ICON_CONSTANT_EDIT,true);
		scr2->addOptionsMenuItem("Menu item 1", MAW_OPTIONS_MENU_ICON_CONSTANT_ADD,true);
	}
}


/**
 * This method is called when the OptionsMenu is being closed
 * (either by the user canceling the menu with the back/menu
 * button, or when an item is selected.
 * @param Screen The screen that generated the event.
 */
void ThirdScreen::optionsMenuClosed(Screen* screen) {

	if ( screen == scr1 )
	{
		ListViewItem* item = new ListViewItem();
		item->setText("Options Menu closed Event on the first stack screen.");
		mListView1->addChild(item);
	}
	else if ( screen == scr2 )
	{
		ListViewItem* item = new ListViewItem();
		item->setText("Options Menu closed Event on the second stack screen.");
		mListView2->addChild(item);
	}
	else if ( screen == mScreen )
	{
		ListViewItem* item = new ListViewItem();
		item->setText("Third screen event: Menu closed.");
		mListView->addChild(item);
	}
}

/**
 * This method is called when an options menu item is selected.
 * @param index The index on which the item is placed into the
 * OptionsMenu.
 *  @param Screen The screen that generated the event.
 */
void ThirdScreen::optionsMenuItemSelected(Screen* screen, int index) {
	if ( screen == scr1 )
	{
		ListViewItem* item = new ListViewItem();
		item->setText("Menu item selected Event on the first screen in stack.index: " + MAUtil::integerToString(index));
		mListView1->addChild(item);
	}
	else if ( screen == scr2 )
	{
		ListViewItem* item = new ListViewItem();
		item->setText("Menu item selected Event on the second screen in stack.index: " + MAUtil::integerToString(index));
		mListView2->addChild(item);
	}
	else if ( screen == mScreen )
	{
		ListViewItem* item = new ListViewItem();
		item->setText("Menu item selected Event on the stack screen.index: " + MAUtil::integerToString(index));
		mListView->addChild(item);
	}
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void ThirdScreen::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
	if ( listView == mListView )
	{
		// Get the index of the clicked item.
		for (int i=0; i < 2; i++)
		{
			if ( mListViewItems[i] == listViewItem )
			{
				openNextScreen(i);
				break;
			}
		}
	}
}
