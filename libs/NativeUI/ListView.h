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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file ListView.h
 * @author Bogdan Iusco
 *
 * \brief An instance of ListView is used for displaying list of information.
 * For list view events see ListViewListener.
 */

#ifndef NATIVEUI_LIST_VIEW_H_
#define NATIVEUI_LIST_VIEW_H_

#include "Widget.h"

namespace NativeUI
{

	/**
	 * @brief List view types.
	 * Platform: iOS.
	 */
	enum ListViewType
	{
		/**
		 * @brief Simple list view.
		 */
		LIST_VIEW_TYPE_DEFAULT = 0,
		/**
		 * @brief Any section headers or footers are displayed as inline separators
		 * and float when the list view is scrolled.
		 */
		LIST_VIEW_TYPE_ALPHABETICAL,
		/**
		 * @brief A list view whose sections present distinct groups of rows.
		 * The section headers and footers do not float.
		 */
		LIST_VIEW_TYPE_SEGMENTED
	};

	/**
	 * @brief List view modes.
	 * Platform: iOS.
	 */
	enum ListViewMode
	{
		/**
		 * @brief List items cannot be edited.
		 */
		LIST_VIEW_MODE_DISPLAY = 0,

		/**
		 * @brief List items can be edited.
		 */
		LIST_VIEW_MODE_EDIT
	};

	/**
	 * @brief List view style.
	 * Platform: Windows Phone 7.
	 */
	enum ListViewStyle
	{
		/**
		 * @brief Every list view item will contain an image, a title and a subtitle.
		 */
		LIST_VIEW_STYLE_SUBTITLE = 0,

		/**
		 * @brief Every list view item will contain an image and a title.
		 */
		LIST_VIEW_STYLE_NO_SUBTITLE
	};

	// Forward declaration.
	class ListViewListener;

	/**
	 * \brief An instance of ListView is used for displaying list of information.
	 * For list view events see ListViewListener.
	 */
	class ListView : public Widget
	{
	public:
		/**
		 * Constructor.
		 * @param type List view's type.
		 */
		ListView(ListViewType type = LIST_VIEW_TYPE_DEFAULT, ListViewStyle style = LIST_VIEW_STYLE_SUBTITLE);

		/**
		 * Destructor.
		 */
		virtual ~ListView();

        /**
         * Enforces the focus on the list.
         * Generally needed when for some reason the list looses it's focus.
         * Platform: Android.
         */
        void requestFocus();

		/**
		 * Get the list view type.
		 * @return List view type.
		 */
		ListViewType getType();

		/**
		 * Get the list view style.
		 * @return List view style.
		 */
		ListViewStyle getStyle();

		/**
		 * Sets the list view style.
		 * This property should be set before the list contains any children.
		 * @param newStyle The new style to be set.
		 */
		void setStyle(ListViewStyle newStyle);

		/**
		 * Set the list view mode.
		 * Platform: iOS.
		 * @param listMode List mode to set.
		 */
		void setMode(ListViewMode listMode);

		/**
		 * Get the list view mode.
		 * Platform: iOS.
		 * @return List mode to get.
		 */
		ListViewMode getMode();

		/**
		 * Reload all its items.
		 * Platform: iOS, Android and Windows Phone 7.
		 */
		void reloadData();

		/**
		 * Enable/disable item selection.
		 * If you don't want to allow the user to select any items, set this
		 * property to false.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @param allow true to enable it, false to disable it.
		 */
		void allowSelection(bool allow = true);

		/**
		 * Check if user is allowed to select an item.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @return true if it's allowed, false otherwise.
		 */
		bool isSelectionAllowed();

		/**
		 * Add an list view event listener.
		 * @param listener The listener that will receive list view events.
		 */
		virtual void addListViewListener(ListViewListener* listener);

		/**
		 * Remove the list view listener.
		 * @param listener The listener that receives list view events.
		 */
		virtual void removeListViewListener(ListViewListener* listener);

	protected:
		/**
		 * This method is called when there is an event for this widget.
		 * It passes on the event to all widget's listeners.
		 * @param widgetEventData The data for the widget event.
		 */
		virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

	private:
		/**
		 * Get the list view type constant.
		 * @param listType Given list type enum.
		 * @return One of the following values:
		 * - MAW_LIST_VIEW_TYPE_DEFAULT
		 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
		 * - MAW_LIST_VIEW_TYPE_SEGMENTED
		 */
		int getListViewTypeFromEnum(ListViewType listType);

		/**
		 * Get the list view type enum from a constant.
		 * @param listType One of the following values:
		 * - MAW_LIST_VIEW_TYPE_DEFAULT
		 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
		 * - MAW_LIST_VIEW_TYPE_SEGMENTED
		 * @return One of the ListViewType enum values.
		 */
		ListViewType getListViewTypeEnum(int listType);

		/**
		 * Get the list view style constant.
		 * @param listStyle Given list style enum.
		 * @return One of the following values:
		 * - MAW_LIST_VIEW_STYLE_SUBTITLE
		 * - MAW_LIST_VIEW_STYLE_NO_SUBTITLE
		 */
		int getListViewStyleFromEnum(ListViewStyle listStyle);

		/**
		 * Get the list view style enum from a constant.
		 * @param listStyle One of the following values:
		 * - MAW_LIST_VIEW_STYLE_SUBTITLE
		 * - MAW_LIST_VIEW_STYLE_NO_SUBTITLE
		 * @return One of the ListViewStyle enum values.
		 */
		ListViewStyle getListViewStyleEnum(int listStyle);

		/**
		 * Notify listeners when receiving #MAW_EVENT_SEGMENTED_LIST_ITEM_INSERT_BTN.
		 * @param widgetEventData Event's data.
		 */
		void notifyListenersClickedInsertButton(MAWidgetEventData* widgetEventData);

		/**
		 * Notify listeners when receiving #MAW_EVENT_SEGMENTED_LIST_ITEM_DELETE_BTN.
		 * @param widgetEventData Event's data.
		 */
		void notifyListenersClickedDeleteButton(MAWidgetEventData* widgetEventData);

	private:
		/**
		 * Array with list view listeners.
		 */
		MAUtil::Vector<ListViewListener*> mListViewListeners;
	};

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_H_ */

/*! @} */
