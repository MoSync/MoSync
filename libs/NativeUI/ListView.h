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

	// Forward declaration.
	class ListViewListener;
	class ListViewSection;

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
		ListView(ListViewType type);

		/**
		 * Destructor.
		 */
		virtual ~ListView();

		/**
		 * Get the list view type.
		 * @return List view type.
		 */
		ListViewType getType();

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
		 * Platform: iOS.
		 */
		void reloadData();

		/**
		 * Enable/disable item selection.
		 * If you don't want to allow the user to select any items, set this
		 * property to false.
		 * Platform: iOS.
		 * @param allow true to enable it, false to disable it.
		 */
		void allowSelection(bool allow = true);

		/**
		 * Check if user is allowed to select an item.
		 * Platform: iOS.
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

	private:
		/**
		 * Array with list view listeners.
		 */
		MAUtil::Vector<ListViewListener*> mListViewListeners;
	};

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_H_ */

/*! @} */
