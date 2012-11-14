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
 * ListScreen.h
 *
 *  Created on: Aug 7, 2012
 *      Author: Spiridon Alexandru
 */

#ifndef LISTSCREEN_H_
#define LISTSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include "ListScreenListener.h"

using namespace NativeUI;

class ListScreen:
	public Screen,
	public ListViewListener,
	public ButtonListener
{

public:
	/**
	 * Constructor.
	 */
	ListScreen();

	/**
	 * Gets the current alphabetical list view.
	 * @return The current alphabetical list view.
	 */
	ListView* getAlphabeticalListView();

	/**
	 * Add a list screen event listener.
	 * @param listener The listener that will receive list screen events.
	 */
	void addListScreenListener(ListScreenListener* listener);

	/**
	 * Remove the list screen listener.
	 * @param listener The listener that receives list screen events.
	 */
	void removeListScreenListener(ListScreenListener* listener);

	/**
	 * Destructor.
	 */
	~ListScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Creates the list manipulation UI (remove section, add/remove item).
	 */
	void createListManipulationLayout();

	/**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param listViewItem The ListViewItem object that was clicked.
	 */
	virtual void listViewItemClicked(
		ListView* listView,
		ListViewItem* listViewItem);

	/**
	 * This method is called when a segmented/alphabetical list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param listViewSection The ListViewSection object that contains the selected item.
	 * @param listViewItem The ListViewItem objet clicked.
	 */
	virtual void segmentedListViewItemClicked(
		ListView* listView,
		ListViewSection* listViewSection,
		ListViewItem* listViewItem);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS, Android and Windows Phone 7.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * The alphabetical list view.
	 */
	ListView* mListView;

	/**
	 * Array with list screen listeners.
	 */
	MAUtil::Vector<ListScreenListener*> mListScreenListeners;

	/**
	 * Reloads the list data.
	 */
	Button* mReloadData;

	/**
	 * Removes a whole section.
	 */
	Button* mRemoveFirstSection;

	/**
	 * Adds an item to the begining of the first section.
	 */
	Button* mAddItem;

	/**
	 * Removes the first item from the first section.
	 */
	Button* mRemoveItem;
};

#endif /* LISTSCREEN_H_ */
