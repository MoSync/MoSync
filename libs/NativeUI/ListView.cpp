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
 * @file ListView.cpp
 * @author Bogdan Iusco
 *
 * An instance of ListView is used for displaying list of information.
 * For list view events see ListViewListener.
 */

#include "ListView.h"
#include "ListViewSection.h"
#include "ListViewListener.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 * @param type List view's type.
	 */
	ListView::ListView(ListViewType type, ListViewStyle style): Widget(MAW_LIST_VIEW)
	{
		this->setPropertyInt(
			MAW_LIST_VIEW_TYPE,
			this->getListViewTypeFromEnum(type));
		this->setPropertyInt(
			MAW_LIST_VIEW_STYLE,
			this->getListViewStyleFromEnum(style));
		this->fillSpaceHorizontally();
		this->fillSpaceVertically();
	}

	/**
	 * Destructor.
	 */
	 ListView::~ListView()
	 {
		 mListViewListeners.clear();
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
	 * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
	 * - #MAW_RES_CANNOT_INSERT_DIALOG if the child is a dialog.
	 * - #MAW_RES_ERROR if it could not be added for some other reason.
	 * @note Since MoSync 3.2 on iOS platform the added widget must be
	 * ListViewItem or ListViewSection. Any other type of widgets will not added.
	 */
	int ListView::addChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

    /**
     * Enforces the focus on the list.
     * Generally needed when for some reason the list looses it's focus.
     * Platform: Android.
     */
    void ListView::requestFocus()
    {
		setProperty(MAW_LIST_VIEW_REQUEST_FOCUS,"");
    }

	 /**
	  * Get the list view type.
	  * @return List view type.
	  */
	 ListViewType ListView::getType()
	 {
		 int type = this->getPropertyInt(MAW_LIST_VIEW_TYPE);
		 return this->getListViewTypeEnum(type);
	 }

	/**
	 * Get the list view style.
	 * @return List view style.
	 */
	ListViewStyle ListView::getStyle()
	{
		int style = this->getPropertyInt(MAW_LIST_VIEW_STYLE);
		return this->getListViewStyleEnum(style);
	}

	/**
	 * Sets the list view style.
	 * This property should be set before the list contains any children.
	 * @param newStyle The new style to be set.
	 */
	void ListView::setStyle(ListViewStyle newStyle)
	{
		int style = this->getListViewStyleFromEnum(newStyle);
		this->setPropertyInt(MAW_LIST_VIEW_STYLE, style);
	}

	 /**
	  * Set the list view mode.
	  * Platform: iOS.
	  * @param listMode List mode to set.
	  */
	 void ListView::setMode(ListViewMode listMode)
	 {
		 int mode = MAW_LIST_VIEW_MODE_DISPLAY;
		 if (listMode == LIST_VIEW_MODE_EDIT)
		 {
			 mode = MAW_LIST_VIEW_MODE_EDIT;
		 }
		 this->setPropertyInt(MAW_LIST_VIEW_MODE, mode);
	 }

	 /**
	  * Get the list view mode.
	  * Platform: iOS.
	  * @return List mode to get.
	  */
	 ListViewMode ListView::getMode()
	 {
		 int mode = this->getPropertyInt(MAW_LIST_VIEW_MODE);
		 ListViewMode listMode = LIST_VIEW_MODE_DISPLAY;
		 if(mode == MAW_LIST_VIEW_MODE_EDIT)
		 {
			 listMode = LIST_VIEW_MODE_EDIT;
		 }
		 return listMode;
	 }

	/**
	 * Reload all its items.
	 * Platform: iOS, Android and Windows Phone 7.
	 */
	void ListView::reloadData()
	{
		this->setProperty(MAW_LIST_VIEW_RELOAD_DATA, "");
	}

	/**
	 * Enable/disable item selection.
	 * If you don't want to allow the user to select any items, set this
	 * property to false.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @param allow true to enable it, false to disable it.
	 */
	void ListView::allowSelection(bool allow)
	{
		const char* value = allow ? "true" : "false";
		this->setProperty(MAW_LIST_VIEW_ALLOW_SELECTION, value);
	}

	/**
	 * Check if user is allowed to select an item.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @return true if it's allowed, false otherwise.
	 */
	bool ListView::isSelectionAllowed()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ALLOW_SELECTION);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	/**
	 * Add an list view event listener.
	 * @param listener The listener that will receive list view events.
	 */
	void ListView::addListViewListener(ListViewListener* listener)
	{
		addListenerToVector(mListViewListeners, listener);
	}

	/**
	 * Remove the list view listener.
	 * @param listener The listener that receives list view events.
	 */
	void ListView::removeListViewListener(ListViewListener* listener)
	{
		removeListenerFromVector(mListViewListeners, listener);
	}

	 /**
	  * This method is called when there is an event for this widget.
	  * It passes on the event to all widget's listeners.
	  * @param widgetEventData The data for the widget event.
	  */
	void ListView::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		Widget::handleWidgetEvent(widgetEventData);

		if (MAW_EVENT_ITEM_CLICKED == widgetEventData->eventType)
		{
			int itemClickedIndex = widgetEventData->listItemIndex;
			ListViewItem* listViewItem = (ListViewItem*)
				this->getChild(itemClickedIndex);

			for (int i = 0; i < mListViewListeners.size(); i++)
			{
				mListViewListeners[i]->listViewItemClicked(
					this,
					listViewItem);
				mListViewListeners[i]->listViewItemClicked(
					this,
					itemClickedIndex);
			}
		}
		else if (MAW_EVENT_SEGMENTED_LIST_ITEM_CLICKED == widgetEventData->eventType)
		{
			int sectionClickedIndex = widgetEventData->sectionIndex;
			int itemIndexWithinSection = widgetEventData->sectionItemIndex;
			ListViewSection* listViewSection = (ListViewSection*)
				this->getChild(sectionClickedIndex);
			ListViewItem* listViewItem = (ListViewItem*)
				listViewSection->getChild(itemIndexWithinSection);

			for (int i = 0; i < mListViewListeners.size(); i++)
			{
				mListViewListeners[i]->segmentedListViewItemClicked(
					this,
					sectionClickedIndex,
					itemIndexWithinSection);
				mListViewListeners[i]->segmentedListViewItemClicked(
					this,
					listViewSection,
					listViewItem);
			}
		}
		else if (MAW_EVENT_SEGMENTED_LIST_ITEM_INSERT_BTN == widgetEventData->eventType)
		{
			this->notifyListenersClickedInsertButton(widgetEventData);
		}
		else if (MAW_EVENT_SEGMENTED_LIST_ITEM_DELETE_BTN == widgetEventData->eventType)
		{
			this->notifyListenersClickedDeleteButton(widgetEventData);
		}
	}

	/**
	 * Get the list view type constant.
	 * @param listType Given list type enum.
	 * @return One of the following values:
	 * - MAW_LIST_VIEW_TYPE_DEFAULT
	 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
	 * - MAW_LIST_VIEW_TYPE_SEGMENTED
	 */
	int ListView::getListViewTypeFromEnum(ListViewType listType)
	{
		int type;
		switch (listType)
		{
		case LIST_VIEW_TYPE_ALPHABETICAL:
			type = MAW_LIST_VIEW_TYPE_ALPHABETICAL;
			break;
		case LIST_VIEW_TYPE_SEGMENTED:
			type = MAW_LIST_VIEW_TYPE_SEGMENTED;
			break;
		case LIST_VIEW_TYPE_DEFAULT:
		default:
			type = MAW_LIST_VIEW_TYPE_DEFAULT;
			break;
		}

		return type;
	}

	/**
	 * Get the list view type enum from a constant.
	 * @param listType One of the following values:
	 * - MAW_LIST_VIEW_TYPE_DEFAULT
	 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
	 * - MAW_LIST_VIEW_TYPE_SEGMENTED
	 * @return One of the ListViewType enum values.
	 */
	ListViewType ListView::getListViewTypeEnum(int listType)
	{
		ListViewType type;
		switch (listType)
		{
		case MAW_LIST_VIEW_TYPE_ALPHABETICAL:
			type = LIST_VIEW_TYPE_ALPHABETICAL;
			break;
		case MAW_LIST_VIEW_TYPE_SEGMENTED:
			type = LIST_VIEW_TYPE_SEGMENTED;
			break;
		case MAW_LIST_VIEW_TYPE_DEFAULT:
		default:
			type = LIST_VIEW_TYPE_DEFAULT;
			break;
		}

		return type;
	}

	/**
	 * Get the list view style constant.
	 * @param listStyle Given list style enum.
	 * @return One of the following values:
	 * - MAW_LIST_VIEW_STYLE_SUBTITLE
	 * - MAW_LIST_VIEW_STYLE_NO_SUBTITLE
	 */
	int ListView::getListViewStyleFromEnum(ListViewStyle listStyle)
	{
		int style;
		switch (listStyle)
		{
		case LIST_VIEW_STYLE_SUBTITLE:
			style = MAW_LIST_VIEW_STYLE_SUBTITLE;
			break;
		case LIST_VIEW_STYLE_NO_SUBTITLE:
			style = MAW_LIST_VIEW_STYLE_NO_SUBTITLE;
			break;
		default:
			style = MAW_LIST_VIEW_STYLE_SUBTITLE;
			break;
		}

		return style;
	}

	/**
	 * Get the list view style enum from a constant.
	 * @param listStyle One of the following values:
	 * - MAW_LIST_VIEW_STYLE_SUBTITLE
	 * - MAW_LIST_VIEW_STYLE_NO_SUBTITLE
	 * @return One of the ListViewStyle enum values.
	 */
	ListViewStyle ListView::getListViewStyleEnum(int listStyle)
	{
		ListViewStyle style;
		switch (listStyle)
		{
		case MAW_LIST_VIEW_STYLE_SUBTITLE:
			style = LIST_VIEW_STYLE_SUBTITLE;
			break;
		case MAW_LIST_VIEW_STYLE_NO_SUBTITLE:
			style = LIST_VIEW_STYLE_NO_SUBTITLE;
			break;
		default:
			style = LIST_VIEW_STYLE_SUBTITLE;
			break;
		}

		return style;
	}

	/**
	 * Notify listeners when receiving #MAW_EVENT_SEGMENTED_LIST_ITEM_INSERT_BTN.
	 * @param widgetEventData Event's data.
	 */
	void ListView::notifyListenersClickedInsertButton(
		MAWidgetEventData* widgetEventData)
	{
		int sectionClickedIndex = widgetEventData->sectionIndex;
		int itemIndexWithinSection = widgetEventData->sectionItemIndex;
		ListViewSection* listViewSection = (ListViewSection*)
			this->getChild(sectionClickedIndex);
		ListViewItem* listViewItem = (ListViewItem*)
			listViewSection->getChild(itemIndexWithinSection);

		for (int i = 0; i < mListViewListeners.size(); i++)
		{
			mListViewListeners[i]->segmentedListViewItemInsert(
				this,
				listViewSection,
				listViewItem);
		}
	}

	/**
	 * Notify listeners when receiving #MAW_EVENT_SEGMENTED_LIST_ITEM_DELETE_BTN.
	 * @param widgetEventData Event's data.
	 */
	void ListView::notifyListenersClickedDeleteButton(
		MAWidgetEventData* widgetEventData)
	{
		int sectionClickedIndex = widgetEventData->sectionIndex;
		int itemIndexWithinSection = widgetEventData->sectionItemIndex;
		ListViewSection* listViewSection = (ListViewSection*)
			this->getChild(sectionClickedIndex);
		ListViewItem* listViewItem = (ListViewItem*)
			listViewSection->getChild(itemIndexWithinSection);

		for (int i = 0; i < mListViewListeners.size(); i++)
		{
			mListViewListeners[i]->segmentedListViewItemDelete(
				this,
				listViewSection,
				listViewItem);
		}
	}

} // namespace NativeUI
