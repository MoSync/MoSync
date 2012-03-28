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
 * @file SegmentedListView.cpp
 * @author Bogdan Iusco
 *
 * @brief A SegmentedListView widget is used to present an indexed/grouped list
 * of items.
 * Available on iOS platform.
 */

#include <NativeUI/WidgetUtil.h>

#include "SegmentedListView.h"
#include "SegmentedListViewSection.h"
#include "SegmentedListViewItem.h"
#include "SegmentedListViewListener.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 * @param listType Type of the list.
	 */
	SegmentedListView::SegmentedListView(SegmentedListViewType listType) :
		Widget(MAW_SEGMENTED_LIST_VIEW)
	{
		int listTypeValue;
		if (listType == SegmentedListViewTypePlain)
		{
			listTypeValue = MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN;
		}
		else if (listType == SegmentedListViewTypeGrouped)
		{
			listTypeValue = MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED;
		}

		this->setPropertyInt(MAW_SEGMENTED_LIST_VIEW_TYPE, listTypeValue);
	}

	/**
	 * Destructor.
	 */
	SegmentedListView::~SegmentedListView()
	{
		mSegmentedListViewListeners.clear();
	}

	/**
	 * Get the list type.
	 * @return One of the SegmentedListViewType values.
	 */
	SegmentedListViewType SegmentedListView::getType()
	{
		int listTypeValue = this->getPropertyInt(MAW_SEGMENTED_LIST_VIEW_TYPE);
		SegmentedListViewType listType;
		if (listTypeValue == MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN)
		{
			listType = SegmentedListViewTypePlain;
		}
		else if (listTypeValue == MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED)
		{
			listType = SegmentedListViewTypeGrouped;
		}

		return listType;
	}

	/**
	 * Set the list mode.
	 * @param listMode Mode to set.
	 */
	void SegmentedListView::setMode(SegmentedListViewMode listMode)
	{
		int listModeValue;
		if (listMode == SegmentedListViewModeDisplay)
		{
			listModeValue = MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY;
		}
		else if (listMode == SegmentedListViewModeEdit)
		{
			listModeValue = MAW_SEGMENTED_LIST_VIEW_MODE_EDIT;
		}

		this->setPropertyInt(MAW_SEGMENTED_LIST_VIEW_MODE, listModeValue);
	}

	/**
	 * Get the list mode.
	 * @return One of the SegmentedListViewMode values.
	 */
	SegmentedListViewMode SegmentedListView::getMode()
	{
		int listModeValue = this->getPropertyInt(MAW_SEGMENTED_LIST_VIEW_MODE);
		SegmentedListViewMode listMode;
		if (listModeValue == MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY)
		{
			listMode = SegmentedListViewModeDisplay;
		}
		else if (listModeValue == MAW_SEGMENTED_LIST_VIEW_MODE_EDIT)
		{
			listMode = SegmentedListViewModeEdit;
		}

		return listMode;
	}

	/**
	 * Reload list view data.
	 * Call this method after each modification done to list view data.
	 * Platform: iOS.
	 */
	void SegmentedListView::reloadData()
	{
		this->setProperty(MAW_SEGMENTED_LIST_VIEW_RELOAD_DATA, "");
	}

	/**
	 * Add a section to this list.
	 * @param section The section that will be added.
	 * The ownership of the section object is passed to this method.
	 */
	void SegmentedListView::addSection(SegmentedListViewSection* section)
	{
		this->addChild(section);
	}

	/**
	 * Add an segmented list view event listener.
	 * @param listener The listener that will receive segmented list view
	 * events.
	 */
	void SegmentedListView::addSegmentedListViewListener(
		SegmentedListViewListener* listener)
	{
		addListenerToVector(mSegmentedListViewListeners, listener);
	}

	/**
	 * Remove the segmented list view listener.
	 * @param listener The listener that receives segmented list view events.
	 */
	void SegmentedListView::removeSegmentedListViewListener(
		SegmentedListViewListener* listener)
	{
		removeListenerFromVector(mSegmentedListViewListeners, listener);
	}

	/**
	 * Add a widget as a child of this widget.
	 * @param widget The widget that will be added.
	 * The ownership of the widget is passed to this function.
	 * When the parent widget will be destroyed, all child widgets
	 * will be deleted.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be added to the parent.
	 * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
	 * - #MAW_RES_ERROR if it could not be added for some other reason.
	 */
	int SegmentedListView::addChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

	/**
	 * Insert a widget as a child of this widget, at a given position.
	 * @param widget The widget to be added.
	 * The ownership of the widget is passed to this function.
	 * When the parent widget will be destroyed, all child widgets
	 * will be deleted.
	 * @param index The position on which to add the child.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be added to the parent.
	 * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
	 * - #MAW_RES_INVALID_INDEX if the index was out of bounds.
	 * - #MAW_RES_ERROR if it could not be added for some other reason.
	 */
	int SegmentedListView::insertChild(Widget* widget, const int index)
	{
		return Widget::insertChild(widget, index);
	}

	/**
	 * Remove a child widget from its parent (but does not destroy it).
	 * Removing a currently visible top-level widget causes the MoSync view
	 * to become visible.
	 * When the parent widget will be destroyed, the child widget will not
	 * be deleted.
	 * @param widget The widget to be removed.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be removed from the parent.
	 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int SegmentedListView::removeChild(Widget* widget)
	{
		return Widget::removeChild(widget);
	}

	/**
	 * This method is called when there is an event for this widget.
	 * It passes on the event to all widget's listeners.
	 * @param widgetEventData The data for the widget event.
	 */
   void SegmentedListView::handleWidgetEvent(MAWidgetEventData* widgetEventData)
   {
		Widget::handleWidgetEvent(widgetEventData);
		if (MAW_EVENT_SEGMENTED_ITEM_WILL_SELECT == widgetEventData->eventType)
		{
			this->itemWillSelect(widgetEventData->listItemIndex);
		}
		else if (MAW_EVENT_SEGMENTED_ITEM_SELECTED == widgetEventData->eventType)
		{
			this->itemSelected(widgetEventData->listItemIndex);
		}
   }

	/**
	 * Get a segmented list item object at a given index.
	 * @param listItemIndex Item index.
	 * @return The item object at the given index, or NULL if the index
	 * is invalid.
	 * The ownership of the result is not passed to the caller.
	 */
	SegmentedListViewItem* SegmentedListView::getItemAtIndex(
		const int listItemIndex)
	{
		int countSections = this->countChildWidgets();
		int countItems = listItemIndex;
		SegmentedListViewItem* selectedItem = NULL;
		for (int i = 0; i < countSections; i++)
		{
			Widget* section = this->getChild(i);
			int tempCountItems = countItems - section->countChildWidgets();
			if (tempCountItems < 0)
			{
				selectedItem = (SegmentedListViewItem*) section->getChild(
					countItems);
				break;
			}
			else
			{
				countItems = tempCountItems;
			}
		}

		return selectedItem;
	}

	/**
	 * Notify observers that an segmented list view item is about to be
	 * selected.
	 * @param segmentedListItemIndex Item index that is about to be selected.
	 */
	void SegmentedListView::itemWillSelect(const int segmentedListItemIndex)
	{
		printf("SegmentedListView::itemWillSelect");
		SegmentedListViewItem* selectedItem =
			this->getItemAtIndex(segmentedListItemIndex);
		if (!selectedItem)
		{
			printf("NULL item");
			return;
		}

		for (int i = 0; i < mSegmentedListViewListeners.size(); i++)
		{
			mSegmentedListViewListeners[i]->segmentedListViewItemWillSelect(
				this,
				selectedItem,
				segmentedListItemIndex);
		}
	}

	/**
	 * Notify observers that an segmented list view item was selected.
	 * @param segmentedListItemIndex Item index that was selected.
	 */
	void SegmentedListView::itemSelected(const int segmentedListItemIndex)
	{
		printf("SegmentedListView::itemSelected");
		SegmentedListViewItem* selectedItem =
			this->getItemAtIndex(segmentedListItemIndex);
		if (!selectedItem)
		{
			printf("NULL item");
			return;
		}

		for (int i = 0; i < mSegmentedListViewListeners.size(); i++)
		{
			mSegmentedListViewListeners[i]->segmentedListViewItemSelected(
				this,
				selectedItem,
				segmentedListItemIndex);
		}
	}
}
