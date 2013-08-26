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
		 * @deprecated
		 * @param listView The list view object that generated the event.
		 * @param listViewItem The ListViewItem object that was clicked.
		 */
		virtual void listViewItemClicked(
			ListView* listView,
			ListViewItem* listViewItem) {};

		/**
		 * This method is called when a list view item is clicked.
		 * @deprecated
		 * @param listView The list view object that generated the event.
		 * @param index The index on which the list view item is positioned.
		 */
		virtual void listViewItemClicked(
			ListView* listView,
			int index) {};

		/**
		 * This method is called when a segmented/alphabetical list view item is clicked.
		 * @deprecated Use #listViewItemClicked instead.
		 * @param listView The list view object that generated the event.
		 * @param sectionIndex The index of the section that contains the selected item.
		 * @param itemIndex The index (within the parent section) of the list view item clicked.
		 */
		virtual void segmentedListViewItemClicked(
			ListView* listView,
			int sectionIndex,
			int itemIndex) {};

		/**
		 * This method is called when a segmented/alphabetical list view item is clicked.
		 * @deprecated Use #listViewItemClicked instead.
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The ListViewSection object that contains the selected item.
		 * @param listViewItem The ListViewItem objet clicked.
		 */
		virtual void segmentedListViewItemClicked(
			ListView* listView,
			ListViewSection* listViewSection,
			ListViewItem* listViewItem) {};

		/**
		 * This method is called when an item's insert button is clicked.
		 * The list type must be segmented and in editing mode.
		 * Platform: iOS
		 * @deprecated Use #listViewItemInsert instead.
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The ListViewSection object that contains the item.
		 * Will be null if list view type is default.
		 * @param listViewItem The item object whose insert button was clicked.
		 */
		virtual void segmentedListViewItemInsert(
			ListView* listView,
			ListViewSection* listViewSection,
			ListViewItem* listViewItem) {};

		/**
		 * This method is called when an item's delete button is clicked.
		 * The list type must be segmented and in editing mode.
		 * Platform: iOS
		 * @deprecated Use #listViewItemDelete instead.
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The ListViewSection object that contains the item.
		 * Will be null if list view type is default.
		 * @param listViewItem The item object whose delete button was clicked.
		 */
		virtual void segmentedListViewItemDelete(
			ListView* listView,
			ListViewSection* listViewSection,
			ListViewItem* listViewItem) {};

		/**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param sectionIndex The index of the section that contains the selected item.
		 * Will be #MAW_RES_INVALID_INDEX for default type list views.
		 * @param itemIndex The index (within the parent section if the section is valid)
		 * of the list view item clicked.
		 */
		virtual void listViewItemClicked(
			ListView *listView,
			const int sectionIndex,
			const int itemIndex) {};

		/**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The section object that contains the selected item.
		 * Will be null for default type list views.
		 * @param listViewItem The item object that was clicked.
		 */
		virtual void listViewItemClicked(
			ListView *listView,
			ListViewSection *listViewSection,
			ListViewItem *listViewItem) {};

		/**
		 * This method is called when an item's insert button is clicked.
		 * The list view must be in editing mode.
		 * Platform: iOS
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The section object that contains the item.
		 * Will be null for default type list views.
		 * @param listViewItem The item object whose insert button was clicked.
		 */
		virtual void listViewItemInsert(
			ListView *listView,
			ListViewSection *listViewSection,
			ListViewItem *listViewItem) {};

		/**
		 * This method is called when an item's delete button is clicked.
		 * The list view must be in editing mode.
		 * Platform: iOS
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The section object that contains the item.
		 * Will be null for default type list views.
		 * @param listViewItem The item object whose delete button was clicked.
		 */
		virtual void listViewItemDelete(
			ListView *listView,
			ListViewSection *listViewSection,
			ListViewItem *listViewItem) {};
	};

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_LISTENER_H_ */

/*! @} */
