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
 * @file SegmentedListView.h
 * @author Bogdan Iusco
 *
 * @brief A SegmentedListView widget is used to present an indexed/grouped list
 * of items.
 * Available on iOS platform.
 */

#ifndef NATIVEUI_SEGMENTED_LIST_VIEW_H_
#define NATIVEUI_SEGMENTED_LIST_VIEW_H_

#include <NativeUI/Widget.h>

namespace NativeUI
{

	/**
	 * @brief A SegmentedListView widget is used to present an indexed/grouped
	 * list of items.
	 * Only SegmentedListViewItem objects can be added to this list.
	 * Available on iOS platform.
	 */
	class SegmentedListView: public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		SegmentedListView();

		/**
		 * Destructor.
		 */
		virtual ~SegmentedListView();
	};

} // namespace NativeUI

#endif /* NATIVEUI_SEGMENTED_LIST_VIEW_H_ */

/*! @} */
