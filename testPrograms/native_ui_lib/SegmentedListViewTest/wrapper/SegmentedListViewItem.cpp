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

	/**
	 * Set the title of the delete-confirmation button.
	 * The table view displays the "Delete" button when the user attempts
	 * to delete a row, either by swiping the row or tapping the red minus
	 * icon in editing mode.
	 * Platform: iOS.
	 * @param title Title to set.
	 */
	void SegmentedListViewItem::setDeleteButtonTitle(
		const MAUtil::String& title)
	{
		this->setProperty(
			MAW_SEGMENTED_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE,
			title);
	}

	/**
	 * Get the title of the delete-confirmation button.
	 * The table view displays the "Delete" button when the user attempts
	 * to delete a row, either by swiping the row or tapping the red minus
	 * icon in editing mode.
	 * Platform: iOS.
	 * @return The title.
	 */
	MAUtil::String SegmentedListViewItem::getDeleteButtonTitle()
	{
		return this->getPropertyString(
				MAW_SEGMENTED_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE);
	}

	/**
	 * Check if the item is currently showing the delete-confirmation button.
	 * When users tap the deletion control (the red circle to the left of
	 * the cell), the cell displays a "Delete" button on the right side of
	 * the cell.
	 * Platform: iOS.
	 * @return True if it's showing the delete confirmation button, false
	 * otherwise.
	 */
	bool SegmentedListViewItem::isShowingDeleteConfirmation()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_SEGMENTED_LIST_VIEW_ITEM_IS_SHOWING_DELETE_CONFIRMATION);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	/**
	 * Set the selected state of the item.
	 * @param state If true the item will be selected, otherwise it will
	 * be unselected.
	 * @param animate If true will animate the transition between selected
	 * states, false to make the transition immediate.
	 * Platform: iOS.
	 */
	void SegmentedListViewItem::setSelected(bool state, bool animate)
	{
		const char* propertyName = state ?
			MAW_SEGMENTED_LIST_VIEW_ITEM_SET_SELECTED :
			MAW_SEGMENTED_LIST_VIEW_ITEM_SET_UNSELECTED;
		const char* value = animate ? "true" : "false";
		this->setProperty(propertyName, value);
	}

	/**
	 * Check if the item is selected.
	 * Platform: iOS.
	 * @return True if selected, false otherwise.
	 */
	bool SegmentedListViewItem::isSelected()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_SEGMENTED_LIST_VIEW_ITEM_IS_SELECTED);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	/**
	 * Set the highlighted state of the item.
	 * @param state If true the item will be highlighted, otherwise it will
	 * be unhighlighted.
	 * @param animate If true will animate the transition between highlighted
	 * states, false to make the transition immediate.
	 * Platform: iOS.
	 */
	void SegmentedListViewItem::setHighlighted(bool state, bool animate)
	{
		const char* propertyName = state ?
			MAW_SEGMENTED_LIST_VIEW_ITEM_SET_HIGHLIGHTED :
			MAW_SEGMENTED_LIST_VIEW_ITEM_SET_UNHIGHLIGHTED;
		const char* value = animate ? "true" : "false";
		this->setProperty(propertyName, value);
	}

	/**
	 * Check if the item is highlighted.
	 * Platform: iOS.
	 * @return True if highlighted, false otherwise.
	 */
	bool SegmentedListViewItem::isHighlighted()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_SEGMENTED_LIST_VIEW_ITEM_IS_HIGHLIGHTED);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	/**
	 * Set the type of standard accessory used by the item.
	 * The accessory view appears in the right side of the cell in the
	 * table view’s normal state.
	 * Platform: iOS.
	 * @param type Accessory type to set.
	 */
	void SegmentedListViewItem::setAccessoryType(
		SegmentedListViewItemAccessoryType type)
	{
		this->setProperty(
			MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE,
			this->getAccessoryTypeFromEnum(type));
	}

	/**
	 * Get the type of standard accessory used by the item.
	 * @return The accessory type.
	 */
	SegmentedListViewItemAccessoryType SegmentedListViewItem::getAccessoryType()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE);
		return this->getAccessoryTypeFromString(value);
	}

	/**
	 * Set the type of standard accessory used by the item in editing state.
	 * The accessory view appears in the right side of the cell in the
	 * table view’s editing state.
	 * Platform: iOS.
	 * @param type Accessory type to set.
	 */
	void SegmentedListViewItem::setAccessoryTypeEditingState(
		SegmentedListViewItemAccessoryType type)
	{
		this->setProperty(
			MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT,
			this->getAccessoryTypeFromEnum(type));
	}

	/**
	 * Get the type of standard accessory used by the item in editing state.
	 * @return The accessory type.
	 */
	SegmentedListViewItemAccessoryType SegmentedListViewItem::getAccessoryTypeEditingState()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT);
		return this->getAccessoryTypeFromString(value);
	}

	/**
	 * Set the editing style of an item.
	 * This applies only for the editing state.
	 * Platform: iOS.
	 * @param editStyle Style to set.
	 */
	void SegmentedListViewItem::setEditStyle(
		SegmentedListViewItemEditStyle editStyle)
	{
		int style = MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_NONE;
		switch(editStyle)
		{
		case SegmentedListViewItemEditStyleInsert:
			style = MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_INSERT;
			break;
		case SegmentedListViewItemEditStyleDelete:
			style = MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_DELETE;
			break;
		case SegmentedListViewItemEditStyleNone:
		default:
			style = MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_NONE;
			break;
		}
		this->setPropertyInt(MAW_SEGMENTED_LIST_VIEW_ITEM_EDITING_STYLE, style);
	}

	/**
	 * Get the editing style.
	 * Platform: iOS.
	 * @return The editing style used.
	 */
	SegmentedListViewItemEditStyle SegmentedListViewItem::getEditStyle()
	{
		SegmentedListViewItemEditStyle editStyle =
			SegmentedListViewItemEditStyleNone;
		int style = this->getPropertyInt(
			MAW_SEGMENTED_LIST_VIEW_ITEM_EDITING_STYLE);
		switch(style)
		{
		case MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_INSERT:
			style = SegmentedListViewItemEditStyleInsert;
			break;
		case MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_DELETE:
			style = SegmentedListViewItemEditStyleDelete;
			break;
		case MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_NONE:
		default:
			style = SegmentedListViewItemEditStyleNone;
			break;
		}
		return editStyle;
	}

	/**
	 * Convert enum to one of the
	 * \link MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE
	 * MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
	 * @param type Enum to convert.
	 * @return The converted value.
	 */
	MAUtil::String SegmentedListViewItem::getAccessoryTypeFromEnum(
		SegmentedListViewItemAccessoryType type)
	{
		int accessoryTypeInt = SegmentedListViewItemAccessoryNone;
		switch(type)
		{
		case SegmentedListViewItemAccessoryDisclosure:
			accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE;
			break;
		case SegmentedListViewItemAccessoryDetail:
			accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL;
			break;
		case SegmentedListViewItemAccessoryCheckmark:
			accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK;
			break;
		case SegmentedListViewItemAccessoryNone:
		default:
			accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE;
			break;
		}

		char buffer[MAX_BUF_SIZE];
		sprintf(buffer, "%d", accessoryTypeInt);
		return buffer;
	}

	/**
	 * Convert a string value to one of the SegmentedListViewItemAccessoryType
	 * values.
	 * @param accessoryTypeString One of the
	 * \link MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE
	 * MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
	 * @return The converted value.
	 */
	SegmentedListViewItemAccessoryType SegmentedListViewItem::getAccessoryTypeFromString(
		MAUtil::String& accessoryTypeString)
	{
		int accessoryTypeInt = MAUtil::stringToInteger(accessoryTypeString);
		SegmentedListViewItemAccessoryType type;
		switch (accessoryTypeInt)
		{
		case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE:
			type = SegmentedListViewItemAccessoryDisclosure;
			break;
		case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL:
			type = SegmentedListViewItemAccessoryDetail;
			break;
		case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK:
			type = SegmentedListViewItemAccessoryCheckmark;
			break;
		case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE:
		default:
			type = SegmentedListViewItemAccessoryNone;
			break;
		}
		return type;
	}
}
