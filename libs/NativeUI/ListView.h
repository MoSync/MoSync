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
		 * ListViewItem or ListViewSection. Any other type of widgets will ignored.
		 */
		virtual int addChild(Widget* widget);

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
		 * Start a batch insert/remove operation
		 * that will animate all the list view
		 * changes when endAnimation() is called.
		 * Platform: iOS
		 */
		void beginAnimation();

		/**
		 * Ends a batch insert/remove operation
		 * that will animate all the list view
		 * changes since beginAnimation() was called.
		 * Platform: iOS
		 */
		void endAnimation();

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

		/**
		 * Notify listeners when receiving #MAW_EVENT_LIST_ITEM_ACCESSORY_CLICKED.
		 * @param widgetEventData Event's data.
		 */
		void notifyListenersClickedAccessoryItem(MAWidgetEventData* widgetEventData);

		/**
		 * Handle movement of the child ListViewItems internally and
		 * notify listeners when receiving #MAW_EVENT_LIST_ITEM_ITEM_MOVED.
		 * @param widgetEventData Event's data.
		 */
		void handleItemMove(MAWidgetEventData* widgetEventData);

	private:
		/**
		 * Array with list view listeners.
		 */
		MAUtil::Vector<ListViewListener*> mListViewListeners;
	};

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_H_ */

/*! @} */
