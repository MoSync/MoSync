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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file SegmentedListViewListener.h
 * @author Bogdan Iusco
 *
 * @brief Listener for SegmentedListView events.
 * Available on iOS platform.
 */

#ifndef NATIVEUI_SEGMENTED_LIST_VIEW_LISTENER_H_
#define NATIVEUI_SEGMENTED_LIST_VIEW_LISTENER_H_

namespace NativeUI
{

	// Forward declaration
	class SegmentedListView;
	class SegmentedListViewItem;

	/**
	 * @brief Listener for SegmentedListView events.
	 */
	class SegmentedListViewListener
	{
	public:
		/**
		 * Called when a segmented list view item is about to be selected.
		 * Platform: iOS.
		 * @param segmentedListView The segmented list view item that
		 * generated the event.
		 * @param segmentedListViewItem List item that will be selected.
		 * @param segmentedListViewItemIndex List item index that will be
		 * selected.
		 */
		virtual void segmentedListViewItemWillSelect(
			SegmentedListView* segmentedListView,
			SegmentedListViewItem* segmentedListViewItem,
			const int segmentedListViewItemIndex) {};

		/**
		 * Called after a segmented list view item is selected.
		 * Platform: iOS.
		 * @param segmentedListView The segmented list view item that
		 * generated the event.
		 * @param segmentedListViewItem List item that was selected.
		 * @param segmentedListViewItemIndex List item index that was selected.
		 */
		virtual void segmentedListViewItemSelected(
			SegmentedListView* segmentedListView,
			SegmentedListViewItem* segmentedListViewItem,
			const int segmentedListViewItemIndex) {};
	};

} // namespace NativeUI

#endif /* NATIVEUI_SEGMENTED_LIST_VIEW_LISTENER_H_ */

/*! @} */
