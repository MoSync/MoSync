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
 * @file ThirdScreen.h
 * @author emma
 *
 */

#ifndef THIRD_SCREEN_H_
#define THIRD_SCREEN_H_

#include <MAUtil/Vector.h>
#include "NativeUI/Widgets.h"

using namespace NativeUI;

class ThirdScreen :
	public StackScreen,
	public ScreenListener,
	public ListViewListener
{
public:

	/**
	 * Constructor.
	 */
	ThirdScreen();

	/**
	 * Destructor.
	 */
	virtual ~ThirdScreen();

	/**
	 * This method is called when an item is clicked in the list widget.
	 * @param listItemIndex The index of the list item that was clicked.
	 */
	void openNextScreen(int listItemIndex);

    /**
     * This method is called when the OptionsMenu is being closed
     * (either by the user canceling the menu with the back/menu
     * button, or when an item is selected.
     * @param Screen The screen that generated the event.
     */
    virtual void optionsMenuClosed(Screen* screen);

    /**
     * This method is called when an options menu item is selected.
     * @param index The index on which the item is placed into the
     * OptionsMenu.
     *  @param Screen The screen that generated the event.
     */
    virtual void optionsMenuItemSelected(Screen* screen, int index);

    /**
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);
private:
    Screen* mScreen;
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
    /**
     * The list view widget.
     */
    ListView* mListView;

    /**
     * List of list view items.
     */
    MAUtil::Vector<ListViewItem*> mListViewItems;
    Screen* scr1;
    ListView* mListView1;
    Screen* scr2;
    ListView* mListView2;
};

#endif
