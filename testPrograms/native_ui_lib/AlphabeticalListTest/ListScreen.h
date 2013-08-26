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
