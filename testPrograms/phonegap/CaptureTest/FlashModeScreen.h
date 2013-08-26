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
 * @file FlashModeScreen.h
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a list filled with available flash mode
 * constants.
 * The user can change current flash mode by selecting a list item.
 */

#ifndef FLASHMODESCREEN_H_
#define FLASHMODESCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <maapi.h>

using namespace NativeUI;
using namespace MAUtil;

/**
 * @brief Screen used for displaying a list filled with available flash mode
 * constants.
 * The user can change current flash mode by selecting a list item.
 */
class FlashModeScreen:
	public Screen,
	public ListViewListener
{
public:
	/**
	 * Constructor.
	 */
		FlashModeScreen();

	/**
	 * Destructor.
	 */
	virtual ~FlashModeScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

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
     * Used for displaying the video quality constants.
     */
    ListView* mListView;

    /**
     * Used for displaying auto flash mode option.
     */
    ListViewItem* mFlashAutoListItem;

    /**
     * Used for displaying on flash mode option.
     */
    ListViewItem* mFlashOnListItem;

    /**
     * Used for displaying off flash mode option.
     */
    ListViewItem* mFlashOffListItem;

    /**
     * Used for storing currently selected list item.
     */
    ListViewItem* mSelectedListItem;
};

#endif /* FLASHMODESCREEN_H_ */
