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
 * @file ListViewListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for ListView events.
 */

#ifndef NATIVEUI_LIST_VIEW_LISTENER_H_
#define NATIVEUI_LIST_VIEW_LISTENER_H_

namespace NativeUI
{

	// Forward declaration.
	class ListView;
	class ListViewItem;
	class ListViewSection;

	/**
	 * \brief Listener for ListView events.
	 */
	class ListViewListener
	{
	public:
		/**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param listViewItem The ListViewItem object that was clicked.
		 */
		virtual void listViewItemClicked(
			ListView* listView,
			ListViewItem* listViewItem) {};

		/**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param index The index on which the list view item is positioned.
		 */
		virtual void listViewItemClicked(
			ListView* listView,
			int index){};

		/**
		 * This method is called when a segmented/alphabetical list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param sectionIndex The index of the section that contains the selected item.
		 * @param itemIndex The index (within the parent section) of the list view item clicked.
		 */
		virtual void segmentedListViewItemClicked(
			ListView* listView,
			int sectionIndex,
			int itemIndex){};

		/**
		 * This method is called when a segmented/alphabetical list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The ListViewSection object that contains the selected item.
		 * @param listViewItem The ListViewItem objet clicked.
		 */
		virtual void segmentedListViewItemClicked(
			ListView* listView,
			ListViewSection* listViewSection,
			ListViewItem* listViewItem){};

		/**
		 * This method is called when an item's insert button is clicked.
		 * The list type must be segmented and in editing mode.
		 * Platform: iOS
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The ListViewSection object that contains the item.
		 * @param listViewItem The item objet whose insert button was clicked.
		 */
		virtual void segmentedListViewItemInsert(
			ListView* listView,
			ListViewSection* listViewSection,
			ListViewItem* listViewItem){};

		/**
		 * This method is called when an item's delete button is clicked.
		 * The list type must be segmented and in editing mode.
		 * Platform: iOS
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The ListViewSection object that contains the item.
		 * @param listViewItem The item objet whose delete button was clicked.
		 */
		virtual void segmentedListViewItemDelete(
			ListView* listView,
			ListViewSection* listViewSection,
			ListViewItem* listViewItem){};

	};

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_LISTENER_H_ */

/*! @} */
