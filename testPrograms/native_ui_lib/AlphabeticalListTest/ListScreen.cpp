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
 * ListScreen.cpp
 *
 *  Created on: Aug 7, 2012
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

	mListView = new ListView(LIST_VIEW_TYPE_ALPHABETICAL);

	// the list view doesn't automatically sort its elements - the
	// developer has to handle the sorting
	for (int i = 0; i <= 4; i++)
	{
		ListViewSection* section = new ListViewSection(LIST_VIEW_SECTION_TYPE_ALPHABETICAL);
		MAUtil::String sectionTitle = "A";
		sectionTitle[0] += i;
		section->setTitle(sectionTitle);
		section->setHeaderText(sectionTitle);
		section->setFooterText("footer");

		mListView->addChild(section);
		for (int j = 0; j <= 20; j++)
		{
			ListViewItem* item = new ListViewItem();
			MAUtil::String itemText = sectionTitle + "0";
			itemText[1] += j;
			item->setText(itemText);
			item->setSubtitle("some subtitle text");
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
	mRemoveFirstSection->setText("Remove first secction");
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
