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
