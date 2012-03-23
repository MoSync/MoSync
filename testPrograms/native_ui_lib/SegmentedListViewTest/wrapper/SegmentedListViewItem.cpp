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
 * @file SegmentedListViewItem.cpp
 * @author Bogdan Iusco
 *
 * @brief A SegmentedListViewItem widget is used to define the attributes and
 * behaviour of the cells/items that appear in SegmentedListView objects.
 * Available on iOS platform.
 */

#include "SegmentedListViewItem.h"


namespace NativeUI
{
	/**
	 * Constructor.
	 */
	SegmentedListViewItem::SegmentedListViewItem() :
		Widget(MAW_SEGMENTED_LIST_VIEW_ITEM)
	{

	}

	/**
	 * Destructor.
	 */
	SegmentedListViewItem::~SegmentedListViewItem()
	{

	}

	/**
	 * Set the editable state.
	 * If a cell is editable it means that the user can insert or delete a cell.
	 * In order to have those options the segmented list must be in edit mode.
	 * Platform: iOS.
	 * @param editable true if you want to allow the cell to be edited,
	 * false otherwise.
	 */
	void SegmentedListViewItem::setEditable(bool editable)
	{
		const char* value = editable ? "true" : "false";
		this->setProperty(MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT, value);
	}

	/**
	 * Get the editable state.
	 * Platform: iOS.
	 * @return true if the cell can be edit, false otherwise.
	 */
	bool SegmentedListViewItem::isEditable()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	/**
	 * Allow the user to move the cell.
	 * The segmented list must be in edit mode in order for the cells to
	 * be moved.
	 * Platform: iOS.
	 * @param canMove true if you want to allow the cell to be moved,
	 * false otherwise.
	 */
	void SegmentedListViewItem::setCanMove(bool canMove)
	{
		const char* value = canMove ? "true" : "false";
		this->setProperty(MAW_SEGMENTED_LIST_VIEW_ITEM_MOVE, value);
	}

	/**
	 * Check if cell can be moved by the user.
	 * Platform: iOS.
	 * @return true if cell can be moved, false otherwise.
	 */
	bool SegmentedListViewItem::canMove()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_SEGMENTED_LIST_VIEW_ITEM_MOVE);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}
}
