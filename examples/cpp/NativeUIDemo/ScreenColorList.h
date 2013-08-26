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
	public ListViewListener,
	public StackScreenListener
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
	 * @param listViewSection The section object that contains the selected item.
	 * Will be null for default type list views.
	 * @param listViewItem The item object that was clicked.
	 */
	virtual void listViewItemClicked(
		ListView *listView,
		ListViewSection *listViewSection,
		ListViewItem *listViewItem);

    /**
     * This method is called when a screen has been popped from a stack
     * screen.
     * @param stackScreen The stack screen object that generated the event.
     * @param fromScreen The screen that was popped from the stack screen.
     * @param toScreen The screen that will be shown.
     */
    virtual void stackScreenScreenPopped(
        StackScreen* stackScreen,
        Screen* fromScreen,
        Screen* toScreen);
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
