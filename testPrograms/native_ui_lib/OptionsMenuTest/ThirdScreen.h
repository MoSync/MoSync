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
