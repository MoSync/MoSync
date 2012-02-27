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

#include "SegmentedListView.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 */
	SegmentedListView::SegmentedListView() :
		Widget(MAW_SEGMENTED_LIST_VIEW)
	{

	}

	/**
	 * Destructor.
	 */
	SegmentedListView::~SegmentedListView()
	{

	}
}
