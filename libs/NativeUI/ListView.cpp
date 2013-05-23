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

#include <conprint.h>

#include "ListView.h"
#include "ListViewSection.h"
#include "ListViewListener.h"

namespace NativeUI
{
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

	 ListView::~ListView()
	 {
		 mListViewListeners.clear();
	 }

	int ListView::addChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

	void ListView::requestFocus()
	{
		setProperty(MAW_LIST_VIEW_REQUEST_FOCUS,"");
	}

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

	void ListView::setStyle(ListViewStyle newStyle)
	{
		int style = this->getListViewStyleFromEnum(newStyle);
		this->setPropertyInt(MAW_LIST_VIEW_STYLE, style);
	}

	void ListView::setMode(ListViewMode listMode)
	{
		 int mode = MAW_LIST_VIEW_MODE_DISPLAY;
		 if (listMode == LIST_VIEW_MODE_EDIT)
		 {
			 mode = MAW_LIST_VIEW_MODE_EDIT;
		 }
		 this->setPropertyInt(MAW_LIST_VIEW_MODE, mode);
	}

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

	void ListView::reloadData()
	{
		this->setProperty(MAW_LIST_VIEW_RELOAD_DATA, "");
	}

	void ListView::allowSelection(bool allow)
	{
		const char* value = allow ? "true" : "false";
		this->setProperty(MAW_LIST_VIEW_ALLOW_SELECTION, value);
	}

	bool ListView::isSelectionAllowed()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ALLOW_SELECTION);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	void ListView::addListViewListener(ListViewListener* listener)
	{
		addListenerToVector(mListViewListeners, listener);
	}

	void ListView::removeListViewListener(ListViewListener* listener)
	{
		removeListenerFromVector(mListViewListeners, listener);
	}

	void ListView::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		Widget::handleWidgetEvent(widgetEventData);
		if (!widgetEventData)
		{
			printf(" widgetEventData should not be null in ListView::handleWidgetEvent");
			return;
		}

		const int eventType = widgetEventData->eventType;
		switch (eventType)
		{
			case MAW_EVENT_ITEM_CLICKED:
				notifyListenersItemClicked(*widgetEventData);
				break;
			case MAW_EVENT_SEGMENTED_LIST_ITEM_CLICKED:
				notifyListenersSectionItemClicked(*widgetEventData);
				break;
			case MAW_EVENT_SEGMENTED_LIST_ITEM_INSERT_BTN:
				notifyListenersInsertButtonClicked(*widgetEventData);
				break;
			case MAW_EVENT_SEGMENTED_LIST_ITEM_DELETE_BTN:
				notifyListenersDeleteButtonClicked(*widgetEventData);
				break;
			case MAW_EVENT_ITEM_WILL_SELECT:
				// no listener method implemented for this event.
				break;
			default:
				printf("Unhandled switch case in ListView::handleWidgetEvent");
				break;
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

	void ListView::notifyListenersItemClicked(
		const MAWidgetEventData& widgetEventData)
	{
		int itemIndex = widgetEventData.listItemIndex;
		int sectionIndex = widgetEventData.listSectionIndex;
		ListViewSection* section = NULL;
		ListViewItem* item = NULL;
		const int countListeners = mListViewListeners.size();
		bool isDefaultList = sectionIndex == MAW_RES_INVALID_INDEX;

		if (isDefaultList)
		{
			// Default list view. Does not contain sections.
			item = (ListViewItem*)getChild(itemIndex);
		}
		else
		{
			// Alphabetical or segmented list view.
			section = (ListViewSection*)getChild(sectionIndex);
			item = (ListViewItem*)section->getChild(itemIndex);
		}

		for (int i = 0; i < countListeners; i++)
		{
			// deprecated methods.
			mListViewListeners[i]->listViewItemClicked(
				this,
				item);
			mListViewListeners[i]->listViewItemClicked(
				this,
				itemIndex);

			// new methods.
			mListViewListeners[i]->listViewItemClicked(
				this,
				sectionIndex,
				itemIndex);
			mListViewListeners[i]->listViewItemClicked(
				this,
				section,
				item);
		}
	}

	/**
	 * Notify listeners when receiving #MAW_EVENT_SEGMENTED_LIST_ITEM_CLICKED.
	 * @deprecated
	 * @param widgetEventData Event's data.
	 */
	void ListView::notifyListenersSectionItemClicked(
		const MAWidgetEventData& widgetEventData)
	{
		int itemIndex = widgetEventData.sectionItemIndex;
		int sectionIndex = widgetEventData.sectionIndex;
		ListViewSection* section = NULL;
		ListViewItem* item = NULL;
		const int countListeners = mListViewListeners.size();
		section = (ListViewSection*)getChild(sectionIndex);
		item = (ListViewItem*)section->getChild(itemIndex);

		for (int i = 0; i < countListeners; i++)
		{
			mListViewListeners[i]->segmentedListViewItemClicked(
				this,
				sectionIndex,
				itemIndex);
			mListViewListeners[i]->segmentedListViewItemClicked(
				this,
				section,
				item);
		}
	}

	void ListView::notifyListenersInsertButtonClicked(
		const MAWidgetEventData& widgetEventData)
	{
		const int itemIndex = widgetEventData.listItemIndex;
		const int sectionIndex = widgetEventData.listSectionIndex;
		ListViewSection* section = NULL;
		ListViewItem* item = NULL;
		const int countListeners = mListViewListeners.size();
		bool isDefaultList = sectionIndex == MAW_RES_INVALID_INDEX;

		if (!isDefaultList)
		{
			section = (ListViewSection*) getChild(sectionIndex);
			item = (ListViewItem*) section->getChild(itemIndex);
		}
		else
		{
			item = (ListViewItem*) getChild(itemIndex);
		}

		for (int i = 0; i < countListeners; i++)
		{
			// deprecated.
			mListViewListeners[i]->segmentedListViewItemInsert(
				this,
				section,
				item);

			// new method.
			mListViewListeners[i]->listViewItemInsert(
				this,
				section,
				item);
		}
	}

	void ListView::notifyListenersDeleteButtonClicked(
		const MAWidgetEventData& widgetEventData)
	{
		const int itemIndex = widgetEventData.listItemIndex;
		const int sectionIndex = widgetEventData.listSectionIndex;
		ListViewSection* section = NULL;
		ListViewItem* item = NULL;
		const int countListeners = mListViewListeners.size();
		bool isDefaultList = sectionIndex == MAW_RES_INVALID_INDEX;

		if (!isDefaultList)
		{
			section = (ListViewSection*) getChild(sectionIndex);
			item = (ListViewItem*) section->getChild(itemIndex);
		}
		else
		{
			item = (ListViewItem*) getChild(itemIndex);
		}

		for (int i = 0; i < countListeners; i++)
		{
			// deprecated
			mListViewListeners[i]->segmentedListViewItemDelete(
				this,
				section,
				item);

			// new method
			mListViewListeners[i]->listViewItemDelete(
				this,
				section,
				item);
		}
	}

} // namespace NativeUI
