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
 * @file SegmentedListViewItem.h
 * @author Bogdan Iusco
 *
 * @brief A SegmentedListViewItem widget is used to define the attributes and
 * behaviour of the cells/items that appear in SegmentedListView objects.
 * Available on iOS platform.
 */

#ifndef NATIVEUI_SEGMENTED_LIST_VIEW_ITEM_H_
#define NATIVEUI_SEGMENTED_LIST_VIEW_ITEM_H_

#include <NativeUI/Widget.h>
#include <MAUtil/String.h>

namespace NativeUI
{

	typedef enum
	{
		/**
		 * @brief The item does not have any accessory type.
		 * This is the default value.
		 * Platform: iOS.
		 */
		SegmentedListViewItemAccessoryNone = 0,
		/**
		 * @brief The item has an accessory control shaped like a regular chevron.
		 * It is intended as a disclosure indicator.
		 * Platform: iOS.
		 */
		SegmentedListViewItemAccessoryDisclosure,
		/**
		 * @brief The item has an accessory control that is a blue button with
		 * a chevron image as content.
		 * It is intended for configuration purposes.
		 * Platform: iOS.
		 */
		SegmentedListViewItemAccessoryDetail,
		/**
		 * @brief The cell has a check mark on its right side.
		 * Platform: iOS.
		 */
		SegmentedListViewItemAccessoryCheckmark
	} SegmentedListViewItemAccessoryType;

	typedef enum
	{
		/**
		 * @brief The item has no editing control.
		 * This is the default value.
		 * Platform: iOS.
		 */
		SegmentedListViewItemEditStyleNone = 0,
		/**
		 * @brief The item has the delete editing control.
		 * This control is a red circle enclosing a minus sign.
		 * Platform: iOS.
		 */
		SegmentedListViewItemEditStyleDelete,
		/**
		 * @brief The item has the insert editing control.
		 * This control is a green circle enclosing a plus sign.
		 * Platform: iOS.
		 */
		SegmentedListViewItemEditStyleInsert
	} SegmentedListViewItemEditStyle;

	/**
	 * @brief A SegmentedListViewItem widget is used to define the attributes
	 * and behaviour of the cells/items that appear in SegmentedListView objects.
	 * Available on iOS platform.
	 */
	class SegmentedListViewItem: public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		SegmentedListViewItem();

		/**
		 * Destructor.
		 */
		virtual ~SegmentedListViewItem();

		/**
		 * Set the editable state.
		 * If a cell is editable it means that the user can insert or delete a cell.
		 * In order to have those options the segmented list must be in edit mode.
		 * Platform: iOS.
		 * @param editable true if you want to allow the cell to be edited,
		 * false otherwise.
		 */
		void setEditable(bool editable);

		/**
		 * Get the editable state.
		 * Platform: iOS.
		 * @return true if the cell can be edit, false otherwise.
		 */
		bool isEditable();

		/**
		 * Allow the user to move the cell.
		 * The segmented list must be in edit mode in order for the cells to
		 * be moved.
		 * Platform: iOS.
		 * @param canMove true if you want to allow the cell to be moved,
		 * false otherwise.
		 */
		void setCanMove(bool canMove);

		/**
		 * Check if cell can be moved by the user.
		 * Platform: iOS.
		 * @return true if cell can be moved, false otherwise.
		 */
		bool canMove();

		/**
		 * Set the title of the delete-confirmation button.
		 * The table view displays the "Delete" button when the user attempts
		 * to delete a row, either by swiping the row or tapping the red minus
		 * icon in editing mode.
		 * Platform: iOS.
		 * @param title Title to set.
		 */
		void setDeleteButtonTitle(const MAUtil::String& title);

		/**
		 * Get the title of the delete-confirmation button.
		 * The table view displays the "Delete" button when the user attempts
		 * to delete a row, either by swiping the row or tapping the red minus
		 * icon in editing mode.
		 * Platform: iOS.
		 * @return The title.
		 */
		MAUtil::String getDeleteButtonTitle();

		/**
		 * Check if the item is currently showing the delete-confirmation button.
		 * When users tap the deletion control (the red circle to the left of
		 * the cell), the cell displays a "Delete" button on the right side of
		 * the cell.
		 * Platform: iOS.
		 * @return True if it's showing the delete confirmation button, false
		 * otherwise.
		 */
		bool isShowingDeleteConfirmation();

		/**
		 * Set the selected state of the item.
		 * @param state If true the item will be selected, otherwise it will
		 * be unselected.
		 * @param animate If true will animate the transition between selected
		 * states, false to make the transition immediate.
		 * Platform: iOS.
		 */
		void setSelected(bool state = true, bool animate = true);

		/**
		 * Check if the item is selected.
		 * Platform: iOS.
		 * @return True if selected, false otherwise.
		 */
		bool isSelected();

		/**
		 * Set the highlighted state of the item.
		 * @param state If true the item will be highlighted, otherwise it will
		 * be unhighlighted.
		 * @param animate If true will animate the transition between highlighted
		 * states, false to make the transition immediate.
		 * Platform: iOS.
		 */
		void setHighlighted(bool state = true, bool animate = true);

		/**
		 * Check if the item is highlighted.
		 * Platform: iOS.
		 * @return True if highlighted, false otherwise.
		 */
		bool isHighlighted();

		/**
		 * Set the type of standard accessory used by the item.
		 * The accessory view appears in the right side of the cell in the
		 * table view’s normal state.
		 * Platform: iOS.
		 * @param type Accessory type to set.
		 */
		void setAccessoryType(SegmentedListViewItemAccessoryType type);

		/**
		 * Get the type of standard accessory used by the item.
		 * @return The accessory type.
		 */
		SegmentedListViewItemAccessoryType getAccessoryType();

		/**
		 * Set the type of standard accessory used by the item in editing state.
		 * The accessory view appears in the right side of the cell in the
		 * table view’s editing state.
		 * Platform: iOS.
		 * @param type Accessory type to set.
		 */
		void setAccessoryTypeEditingState(
			SegmentedListViewItemAccessoryType type);

		/**
		 * Get the type of standard accessory used by the item in editing state.
		 * @return The accessory type.
		 */
		SegmentedListViewItemAccessoryType getAccessoryTypeEditingState();

		/**
		 * Set the editing style of an item.
		 * This applies only for the editing state.
		 * Platform: iOS.
		 * @param editStyle Style to set.
		 */
		void setEditStyle(SegmentedListViewItemEditStyle editStyle);

		/**
		 * Get the editing style.
		 * Platform: iOS.
		 * @return The editing style used.
		 */
		SegmentedListViewItemEditStyle getEditStyle();

		/**
		 * Show/hide the reorder control.
		 * The reordering control is gray, multiple horizontal bar control
		 * on the right side of the cell. Users can drag this control to reorder
		 * the cell within the table.
		 * The list must be in edit mode.
		 * Platform: iOS.
		 * @param show If true shows the reorder control, false hides it.
		 */
		void showReorderControl(bool show = true);

		/**
		 * Check if the reorder control is shown.
		 * The reordering control is gray, multiple horizontal bar control
		 * on the right side of the cell.
		 * Platform: iOS.
		 * @return true if it's shown, false otherwise.
		 */
		bool isReorderControlShown();

	private:
		/**
		 * Convert enum to one of the
		 * \link MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE
		 * MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
		 * @param type Enum to convert.
		 * @return The converted value.
		 */
		MAUtil::String getAccessoryTypeFromEnum(
			SegmentedListViewItemAccessoryType type);

		/**
		 * Convert a string value to one of the SegmentedListViewItemAccessoryType
		 * values.
		 * @param accessoryTypeString One of the
		 * \link MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE
		 * MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
		 * @return The converted value.
		 */
		SegmentedListViewItemAccessoryType getAccessoryTypeFromString(
			MAUtil::String& accessoryTypeString);
	};

} // namespace NativeUI

#endif /* NATIVEUI_SEGMENTED_LIST_VIEW_ITEM_H_ */

/*! @} */
