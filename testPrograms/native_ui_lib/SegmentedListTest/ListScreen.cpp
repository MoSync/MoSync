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
 * ListScreen.cpp
 *
 *  Created on: Sept 28, 2012
 *      Author: Spiridon Alexandru
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <NativeUI/ListViewSection.h>

#include "ListScreen.h"
#include "ListScreenListener.h"
#include "Util.h"

/**
 * Constructor.
 */
ListScreen::ListScreen() :
	Screen(),
	mMainLayout(NULL),
	mListView(NULL),
	mRemoveFirstSection(NULL),
	mAddItem(NULL),
	mRemoveItem(NULL),
	mReloadData(NULL)
{
	createMainLayout();

	mListView->addListViewListener(this);
	mRemoveFirstSection->addButtonListener(this);
	mAddItem->addButtonListener(this);
	mRemoveItem->addButtonListener(this);
	if (isIOS() || isAndroid())
	{
		mReloadData->addButtonListener(this);
	}
}

/**
 * Destructor.
 */
ListScreen::~ListScreen()
{
	mListView->removeListViewListener(this);
	mRemoveFirstSection->removeButtonListener(this);
	mAddItem->removeButtonListener(this);
	mRemoveItem->removeButtonListener(this);
	if (isIOS() || isAndroid())
	{
		mReloadData->removeButtonListener(this);
	}
	mListScreenListeners.clear();
}

/**
 * Creates and adds main layout to the screen.
 */
void ListScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	mListView = new ListView(LIST_VIEW_TYPE_SEGMENTED);

	// the list view doesn't automatically sort its elements - the
	// developer has to handle the sorting
	for (int i = 0; i <= 5; i++)
	{
		ListViewSection* section = new ListViewSection(LIST_VIEW_SECTION_TYPE_SEGMENTED);
		MAUtil::String sectionTitle = "Header ";
		sectionTitle += MAUtil::integerToString(i);
		section->setTitle("H" + MAUtil::integerToString(i));
		section->setHeaderText(sectionTitle);
		section->setFooterText("Footer " + MAUtil::integerToString(i));

		mListView->addChild(section);
		for (int j = 0; j <= 6; j++)
		{
			ListViewItem* item = new ListViewItem();
			MAUtil::String itemText = "Item ";
			itemText += MAUtil::integerToString(j);
			item->setText(itemText);
			item->setSubtitle("subtitle " + MAUtil::integerToString(j));
			item->fillSpaceHorizontally();
			section->addItem(item);
		}

	}

	int result = mMainLayout->addChild(mListView);
    printf("add mListView result = %d", result);

    createListManipulationLayout();

    if (isIOS() || isAndroid())
    {
		mReloadData = new Button();
		mReloadData->setText("Reload data");
		mReloadData->fillSpaceHorizontally();
		mMainLayout->addChild(mReloadData);
    }
}

/**
 * Creates the list manipulation UI (remove section, add/remove item).
 */
void ListScreen::createListManipulationLayout()
{
	mRemoveFirstSection = new Button();
	mRemoveFirstSection->setText("Remove first section");
	mRemoveFirstSection->fillSpaceHorizontally();
	mMainLayout->addChild(mRemoveFirstSection);

	HorizontalLayout* itemManipulationLayout = new HorizontalLayout();
	itemManipulationLayout->wrapContentVertically();
	mAddItem = new Button();
	mAddItem->setText("Add item");
	mAddItem->fillSpaceHorizontally();
	itemManipulationLayout->addChild(mAddItem);

	mRemoveItem = new Button();
	mRemoveItem->setText("Remove item");
	mRemoveItem->fillSpaceHorizontally();
	itemManipulationLayout->addChild(mRemoveItem);

	mMainLayout->addChild(itemManipulationLayout);
}

/**
 * Gets the current alphabetical list view.
 * @return The current alphabetical list view.
 */
ListView* ListScreen::getAlphabeticalListView()
{
	return mListView;
}

/**
 * Add a list screen event listener.
 * @param listener The listener that will receive list screen events.
 */
void ListScreen::addListScreenListener(ListScreenListener* listener)
{
    for (int i = 0; i < mListScreenListeners.size(); i++)
    {
        if (listener == mListScreenListeners[i])
        {
            return;
        }
    }

    mListScreenListeners.add(listener);
}

/**
 * Remove the list screen listener.
 * @param listener The listener that receives list screen events.
 */
void ListScreen::removeListScreenListener(ListScreenListener* listener)
{
    for (int i = 0; i < mListScreenListeners.size(); i++)
    {
        if (listener == mListScreenListeners[i])
        {
			mListScreenListeners.remove(i);
            break;
        }
    }
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void ListScreen::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
    if (listView == mListView)
    {
        printf("ListScreen: mListView event: item clicked");
    }
}

/**
 * This method is called when an item's insert button is clicked.
 * The list type must be segmented and in editing mode.
 * Platform: iOS
 * @param listView The list view object that generated the event.
 * @param listViewSection The ListViewSection object that contains the item.
 * @param listViewItem The item objet whose insert button was clicked.
 */
void ListScreen::segmentedListViewItemInsert(
	ListView* listView,
	ListViewSection* listViewSection,
	ListViewItem* listViewItem)
{
	maAlert("List View", "Insert button was clicked", "Ok", NULL, NULL);
}

/**
 * This method is called when an item's delete button is clicked.
 * The list type must be segmented and in editing mode.
 * Platform: iOS
 * @param listView The list view object that generated the event.
 * @param listViewSection The ListViewSection object that contains the item.
 * @param listViewItem The item objet whose delete button was clicked.
 */
void ListScreen::segmentedListViewItemDelete(
	ListView* listView,
	ListViewSection* listViewSection,
	ListViewItem* listViewItem)
{
	maAlert("List View", "Delete button was clicked", "Ok", NULL, NULL);
}

/**
 * This method is called when a segmented/alphabetical list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewSection The ListViewSection object that contains the selected item.
 * @param listViewItem The ListViewItem objet clicked.
 */
void ListScreen::segmentedListViewItemClicked(
	ListView* listView,
	ListViewSection* listViewSection,
	ListViewItem* listViewItem)
{
	for (int i = 0; i < mListScreenListeners.size(); i++)
	{
		mListScreenListeners[i]->listScreenItemClicked(listViewSection,
			listViewItem);
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS, Android and Windows Phone 7.
 * @param button The button object that generated the event.
 */
void ListScreen::buttonClicked(Widget* button)
{
	if (button == mReloadData)
	{
		mListView->reloadData();
	}
	else if (button == mRemoveFirstSection)
	{
		if (mListView->countChildWidgets() > 0)
		{
			mListView->removeChild(mListView->getChild(0));
		}
	}
	else if (button == mRemoveItem)
	{
		if (mListView->countChildWidgets() > 0)
		{
			ListViewSection* firstSection = (ListViewSection*)mListView->getChild(0);
			if (firstSection->countChildWidgets() > 0)
			{
				firstSection->removeItem(0);
			}
		}
	}
	else if (button == mAddItem)
	{
		if (mListView->countChildWidgets() > 0)
		{
			ListViewSection* firstSection = (ListViewSection*)mListView->getChild(0);
			ListViewItem* newItem = new ListViewItem();
			newItem->setText("testItem");
			newItem->setSubtitle("subtitle");
			firstSection->insertItem(newItem, 0);
		}
	}
}
