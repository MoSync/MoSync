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
 * @file ScreenColorList.h
 * @author Mikael Kindborg
 *
 * Screen that displays a list of colors. When selecting an item
 * in the list, another screen with that color is displayed.
 */

#ifndef SCREEN_COLOR_LIST_H_
#define SCREEN_COLOR_LIST_H_

#include <MAUtil/Vector.h>
#include "NativeUI/Widgets.h"

using namespace NativeUI;

/**
 * The main screen for the color list.
 */
class ScreenColorList :
	public StackScreen,
	public ListViewListener
{
public:

	/**
	 * Constructor.
	 */
	ScreenColorList();

	/**
	 * Destructor.
	 */
	virtual ~ScreenColorList();

	/**
	 * Create the UI for the color list screen.
	 */
	void createUI();

	/**
	 * Create a colored image.
	 * @param color The color of the image (a hex value).
	 * @param width The width of the image.
	 * @param height The height of the image.
	 * @return Handle to the image. The image needs to be
	 * deallocated with maDestoryObject.
	 */
	MAHandle createColorImage(int color, int width, int height);

	/**
	 * This method is called when an item is clicked in the list widget.
	 * @param listItemIndex The index of the list item that was clicked.
	 */
	void openColorScreen(int listItemIndex);

    /**
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);

private:
    /**
     * The list view widget.
     */
    ListView* mListView;

    /**
     * List of list view items.
     */
    MAUtil::Vector<ListViewItem*> mListViewItems;
};

#endif
