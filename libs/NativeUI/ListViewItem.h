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
 * @file ListViewItem.h
 * @author Bogdan Iusco
 *
 * \brief The ListViewItem class defines the attributes and behavior of the
 * items that appear in ListView objects.
 */

#ifndef NATIVEUI_LIST_VIEW_ITEM_H_
#define NATIVEUI_LIST_VIEW_ITEM_H_

#include "Widget.h"

namespace NativeUI
{

	/**
	 * The ListViewItem class defines the attributes and behavior of the
	 * items that appear in ListView objects.
	 */
	enum ListViewItemAccessory
	{
		LIST_VIEW_ITEM_ACCESSORY_NONE = 0,
		LIST_VIEW_ITEM_ACCESSORY_DISCLOSURE,
		LIST_VIEW_ITEM_ACCESSORY_DETAIL,
		LIST_VIEW_ITEM_ACCESSORY_CHECKMARK
	};

	enum ListViewItemEditStyle
	{
		/**
		 * @brief The item has no editing control.
		 * This is the default value.
		 * Platform: iOS.
		 */
		LIST_VIEW_ITEM_EDIT_STYLE_NONE = 0,
		/**
		 * @brief The item has the delete editing control.
		 * This control is a red circle enclosing a minus sign.
		 * Platform: iOS.
		 */
		LIST_VIEW_ITEM_EDIT_STYLE_DELETE,
		/**
		 * @brief The item has the insert editing control.
		 * This control is a green circle enclosing a plus sign.
		 * Platform: iOS.
		 */
		LIST_VIEW_ITEM_EDIT_STYLE_INSERT
	};

	enum ListViewItemSelectionStyle
	{
		/**
		 * @brief The item has no distinct style for when it is selected.
		 * Platform: iOS.
		 */
		LIST_VIEW_ITEM_SELECTION_STYLE_NONE = 0,
		/**
		 * @brief The item has a blue background for selected state.
		 * This is the default value.
		 * Platform: iOS.
		 */
		LIST_VIEW_ITEM_SELECTION_STYLE_BLUE,
		/**
		 * @brief Then item has a gray background for selected state.
		 * Platform: iOS.
		 */
		LIST_VIEW_ITEM_SELECTION_STYLE_GRAY
	};

	/**
	 * \brief Class for list view items.
	 */
	class ListViewItem : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		ListViewItem();

		/**
		 * Destructor.
		 */
		virtual ~ListViewItem();

		/**
		 * Set the text part of the list view item. If there is an icon the
		 * text will be placed to the right of it, otherwise near the left edge.
		 * @param text The given text.
		 */
		virtual void setText(const MAUtil::String& text);

		/**
		 * Sets the subtitle part of the list view item.
		 * The subtitle will be placed bellow the item text.
		 * @param subtitle The given subtitle.
		 */
		virtual void setSubtitle(const MAUtil::String& subtitle);

		/**
		 * Set an icon of the list view item that is placed to the left
		 * of the text.
		 * @param icon MoSync handle to an uncompressed image resource.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the icon handle was invalid.
		 */
		virtual int setIcon(MAHandle icon);

		/**
		 * Set the type of accessory.
		 * Available only on iOS.
		 * Provides the corresponding icon in the right side of the widget.
		 * @param type The type of the accessory.
		 */
		virtual void setAccessoryType(ListViewItemAccessory type);

		/**
		 * Get the accessory type used by the item.
		 * @return The accessory type.
		 */
		ListViewItemAccessory getAccessoryType();

		/**
		 * Set the font color of the item's text.
		 * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
		 *			  red, green and blue components respectively.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
		 */
		virtual int setFontColor(const int color);

		/**
		 * Set the font size in points of the item's text.
		 * @param size A float that specifies the number of pixels for Android
		 * and WP7, and number of points on iOS.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
		 */
		virtual int setFontSize(const float size);

		/**
		* Set the typeface and style in which the text should be displayed.
		* @param fontHandle A font handle received from loading fonts using
		*  #maFontGetName and #maFontLoadWithName syscalls.
		* @return Any of the following result codes:
		* - #MAW_RES_OK if the property could be set.
		* - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		* - #MAW_RES_ERROR otherwise.
		* @par Example
		* \code
		*   int nrFonts = maFontGetCount();
		*	char buf[256];
		*	// Get first font name.
		*	maFontGetName(1, buf, 256);
		*	// Load that font for size 10.
		*	int fontHandle = maFontLoadWithName(buf, 10);
		*	// Set that font to the label.
		*	maWidgetSetProperty(
		*		labelHandle, MAW_LABEL_FONT_HANDLE, toString(fontHandle));
		* \endcode
		*/
		virtual int setFont(const MAHandle fontHandle);

		/**
		 * Set the editable state.
		 * If a cell is editable it means that the user can insert or delete a cell.
		 * In order to have those options the list must be in edit mode.
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
		 * Set the title of the delete-confirmation button.
		 * The list view displays the "Delete" button when the user attempts
		 * to delete an item, either by swiping the item or tapping the red minus
		 * icon in editing mode.
		 * Platform: iOS.
		 * @param title Title to set.
		 */
		void setDeleteButtonTitle(const MAUtil::String& title);

		/**
		 * Get the title of the delete-confirmation button.
		 * The list view displays the "Delete" button when the user attempts
		 * to delete an item, either by swiping the item or tapping the red minus
		 * icon in editing mode.
		 * Platform: iOS.
		 * @return The title.
		 */
		MAUtil::String getDeleteButtonTitle();

		/**
		 * Check if the item is currently showing the delete-confirmation button.
		 * When users tap the deletion control (the red circle to the left of
		 * the item), it displays a "Delete" button on the right side of
		 * the item.
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
		 * Platform: iOS and Android.
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
		 * Set the type of standard accessory used by the item in editing state.
		 * The accessory view appears in the right side of the item in the
		 * list view�s editing state.
		 * Platform: iOS.
		 * @param type Accessory type to set.
		 */
		void setAccessoryTypeEditingState(
			ListViewItemAccessory type);

		/**
		 * Get the type of standard accessory used by the item in editing state.
		 * @return The accessory type.
		 */
		ListViewItemAccessory getAccessoryTypeEditingState();

		/**
		 * Set the editing style of an item.
		 * This applies only for the editing state.
		 * Platform: iOS.
		 * @param editStyle Style to set.
		 */
		void setEditStyle(ListViewItemEditStyle editStyle);

		/**
		 * Get the editing style.
		 * Platform: iOS.
		 * @return The editing style used.
		 */
		ListViewItemEditStyle getEditStyle();

		/**
		 * Set the predefined background color of an selected list view item.
		 * @param selectionStyle Predefined styles.
		 * The default value is LIST_VIEW_ITEM_SELECTION_STYLE_BLUE.
		 * Platform: iOS.
		 */
		void setSelectionStyle(ListViewItemSelectionStyle selectionStyle);

		/**
		 * Get the predefined background color of an selected list view item.
		 * Platform: iOS.
		 * @return The predefined style.
		 */
		ListViewItemSelectionStyle getSelectionStyle();

	private:
		/**
		 * Convert enum to one of the
		 * \link MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE
		 * MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
		 * @param type Enum to convert.
		 * @return The converted value.
		 */
		int getAccessoryTypeFromEnum(
			ListViewItemAccessory type);

		/**
		 * Convert a string value to one of the ListViewItemAccessory values.
		 * @param accessoryTypeString One of the
		 * \link MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE
		 * MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
		 * @return The converted value.
		 */
		ListViewItemAccessory getAccessoryTypeFromString(
			MAUtil::String& accessoryTypeString);

	};

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_ITEM_H_ */

/*! @} */
