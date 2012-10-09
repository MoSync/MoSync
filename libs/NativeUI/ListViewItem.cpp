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

/**
 * @file ListViewItem.cpp
 * @author Bogdan Iusco
 *
 * The ListViewItem class defines the attributes and behavior of the items
 * that appear in ListView objects.
 */

#include "ListViewItem.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 */
	ListViewItem::ListViewItem(): Widget(MAW_LIST_VIEW_ITEM)
	{
	}

	/**
	 * Destructor.
	 */
	ListViewItem::~ListViewItem()
	 {
	 }

	/**
	 * Set the text part of the list view item. If there is an icon the
	 * text will be placed to the right of it, otherwise near the left edge.
	 * @param text The given text.
	 */
	void ListViewItem::setText(const MAUtil::String& text)
	{
		this->setProperty(MAW_LIST_VIEW_ITEM_TEXT, text);
	}

	/**
	 * Sets the subtitle part of the list view item.
	 * The subtitle will be placed bellow the item text.
	 * @param subtitle The given subtitle.
	 */
	void ListViewItem::setSubtitle(const MAUtil::String& subtitle)
	{
		this->setProperty(MAW_LIST_VIEW_ITEM_SUBTITLE, subtitle);
	}

	/**
	 * Set an icon of the list view item that is placed to the left
	 * of the text.
	 * @param icon MoSync handle to an uncompressed image resource.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the icon handle was invalid.
	 */
	int ListViewItem::setIcon(MAHandle icon)
	{
		return this->setPropertyInt(MAW_LIST_VIEW_ITEM_ICON, icon);
	}

	/**
	 * Set the type of accessory.
	 * Available only on iOS.
	 * Provides the corresponding icon in the right side of the widget.
	 * @param type The type of the accessory.
	 */
	void ListViewItem::setAccessoryType(ListViewItemAccessory type)
	{
		this->setPropertyInt(MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT,
			this->getAccessoryTypeFromEnum(type));
	}

	/**
	 * Get the accessory type used by the item.
	 * @return The accessory type.
	 */
	ListViewItemAccessory ListViewItem::getAccessoryType()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT);
		return this->getAccessoryTypeFromString(value);
	}

	/**
	 * Set the font color of the item's text.
	 * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
	 *			  red, green and blue components respectively.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
	 */
	int ListViewItem::setFontColor(const int color)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "0x%.6X", color);
		return this->setProperty(MAW_LIST_VIEW_ITEM_FONT_COLOR, buffer);
	}

	/**
	 * Set the font size in points of the item's text.
	 * @param size A float that specifies the number of pixels for Android
	 * and WP7, and number of points on iOS.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
	 */
	int ListViewItem::setFontSize(const float size)
	{
		return this->setPropertyFloat(MAW_LIST_VIEW_ITEM_FONT_SIZE, size);
	}

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
	int ListViewItem::setFont(const MAHandle fontHandle)
	{
		return this->setPropertyInt(MAW_LIST_VIEW_ITEM_FONT_HANDLE, fontHandle);
	}

	/**
	 * Set the editable state.
	 * If a cell is editable it means that the user can insert or delete a cell.
	 * In order to have those options the list must be in edit mode.
	 * Platform: iOS.
	 * @param editable true if you want to allow the cell to be edited,
	 * false otherwise.
	 */
	void ListViewItem::setEditable(bool editable)
	{
		const char* value = editable ? "true" : "false";
		this->setProperty(MAW_LIST_VIEW_ITEM_EDIT, value);
	}

	/**
	 * Get the editable state.
	 * Platform: iOS.
	 * @return true if the cell can be edit, false otherwise.
	 */
	bool ListViewItem::isEditable()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ITEM_EDIT);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	/**
	 * Set the title of the delete-confirmation button.
	 * The list view displays the "Delete" button when the user attempts
	 * to delete an item, either by swiping the item or tapping the red minus
	 * icon in editing mode.
	 * Platform: iOS.
	 * @param title Title to set.
	 */
	void ListViewItem::setDeleteButtonTitle(
		const MAUtil::String& title)
	{
		this->setProperty(MAW_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE, title);
	}

	/**
	 * Get the title of the delete-confirmation button.
	 * The list view displays the "Delete" button when the user attempts
	 * to delete an item, either by swiping the item or tapping the red minus
	 * icon in editing mode.
	 * Platform: iOS.
	 * @return The title.
	 */
	MAUtil::String ListViewItem::getDeleteButtonTitle()
	{
		return this->getPropertyString(MAW_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE);
	}

	/**
	 * Check if the item is currently showing the delete-confirmation button.
	 * When users tap the deletion control (the red circle to the left of
	 * the item), it displays a "Delete" button on the right side of
	 * the item.
	 * Platform: iOS.
	 * @return True if it's showing the delete confirmation button, false
	 * otherwise.
	 */
	bool ListViewItem::isShowingDeleteConfirmation()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ITEM_IS_SHOWING_DELETE_CONFIRMATION);
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
	void ListViewItem::setSelected(bool state, bool animate)
	{
		const char* propertyName = state ?
			MAW_LIST_VIEW_ITEM_SET_SELECTED :
			MAW_LIST_VIEW_ITEM_SET_UNSELECTED;
		const char* value = animate ? "true" : "false";
		this->setProperty(propertyName, value);
	}

	/**
	 * Check if the item is selected.
	 * Platform: iOS and Android.
	 * @return True if selected, false otherwise.
	 */
	bool ListViewItem::isSelected()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ITEM_IS_SELECTED);
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
	void ListViewItem::setHighlighted(bool state, bool animate)
	{
		const char* propertyName = state ?
			MAW_LIST_VIEW_ITEM_SET_HIGHLIGHTED :
			MAW_LIST_VIEW_ITEM_SET_UNHIGHLIGHTED;
		const char* value = animate ? "true" : "false";
		this->setProperty(propertyName, value);
	}

	/**
	 * Check if the item is highlighted.
	 * Platform: iOS.
	 * @return True if highlighted, false otherwise.
	 */
	bool ListViewItem::isHighlighted()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ITEM_IS_HIGHLIGHTED);
		bool returnValue = (strcmp(value.c_str(), "true")) ? false : true;
		return returnValue;
	}

	/**
	 * Set the type of standard accessory used by the item in editing state.
	 * The accessory view appears in the right side of the item in the
	 * list viewÕs editing state.
	 * Platform: iOS.
	 * @param type Accessory type to set.
	 */
	void ListViewItem::setAccessoryTypeEditingState(ListViewItemAccessory type)
	{
		this->setPropertyInt(MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT,
			this->getAccessoryTypeFromEnum(type));
	}

	/**
	 * Get the type of standard accessory used by the item in editing state.
	 * @return The accessory type.
	 */
	ListViewItemAccessory ListViewItem::getAccessoryTypeEditingState()
	{
		MAUtil::String value = this->getPropertyString(
			MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT);
		return this->getAccessoryTypeFromString(value);
	}

	/**
	 * Set the editing style of an item.
	 * This applies only for the editing state.
	 * Platform: iOS.
	 * @param editStyle Style to set.
	 */
	void ListViewItem::setEditStyle(ListViewItemEditStyle editStyle)
	{
		int style = MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE;
		switch(editStyle)
		{
		case LIST_VIEW_ITEM_EDIT_STYLE_INSERT:
			style = MAW_LIST_VIEW_ITEM_EDIT_STYLE_INSERT;
			break;
		case LIST_VIEW_ITEM_EDIT_STYLE_DELETE:
			style = MAW_LIST_VIEW_ITEM_EDIT_STYLE_DELETE;
			break;
		case LIST_VIEW_ITEM_EDIT_STYLE_NONE:
		default:
			style = MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE;
			break;
		}
		this->setPropertyInt(MAW_LIST_VIEW_ITEM_EDITING_STYLE, style);
	}

	/**
	 * Get the editing style.
	 * Platform: iOS.
	 * @return The editing style used.
	 */
	ListViewItemEditStyle ListViewItem::getEditStyle()
	{
		ListViewItemEditStyle editStyle = LIST_VIEW_ITEM_EDIT_STYLE_NONE;
		int style = this->getPropertyInt(MAW_LIST_VIEW_ITEM_EDITING_STYLE);

		switch(style)
		{
		case MAW_LIST_VIEW_ITEM_EDIT_STYLE_INSERT:
			editStyle = LIST_VIEW_ITEM_EDIT_STYLE_INSERT;
			break;
		case MAW_LIST_VIEW_ITEM_EDIT_STYLE_DELETE:
			editStyle = LIST_VIEW_ITEM_EDIT_STYLE_DELETE;
			break;
		case MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE:
		default:
			editStyle = LIST_VIEW_ITEM_EDIT_STYLE_NONE;
			break;
		}
		return editStyle;
	}

	/**
	 * Set the predefined background color of an selected list view item.
	 * @param selectionStyle Predefined styles.
	 * The default value is LIST_VIEW_ITEM_SELECTION_STYLE_BLUE.
	 * Platform: iOS.
	 */
	void ListViewItem::setSelectionStyle(ListViewItemSelectionStyle selectionStyle)
	{
		int selectionStyleInt;

		switch (selectionStyle)
		{
		case LIST_VIEW_ITEM_SELECTION_STYLE_GRAY:
			selectionStyleInt = MAW_LIST_VIEW_ITEM_SELECTION_STYLE_GRAY;
			break;
		case LIST_VIEW_ITEM_SELECTION_STYLE_NONE:
			selectionStyleInt = MAW_LIST_VIEW_ITEM_SELECTION_STYLE_NONE;
			break;
		case LIST_VIEW_ITEM_SELECTION_STYLE_BLUE:
		default:
			selectionStyleInt = MAW_LIST_VIEW_ITEM_SELECTION_STYLE_BLUE;
			break;

		}

		this->setPropertyInt(MAW_LIST_VIEW_ITEM_SELECTION_STYLE, selectionStyleInt);
	}

	/**
	 * Get the predefined background color of an selected list view item.
	 * Platform: iOS.
	 * @return The predefined style.
	 */
	ListViewItemSelectionStyle ListViewItem::getSelectionStyle()
	{
		ListViewItemSelectionStyle style;
		int selectionStyleInt = this->getPropertyInt(
			MAW_LIST_VIEW_ITEM_SELECTION_STYLE);

		switch (selectionStyleInt)
		{
		case MAW_LIST_VIEW_ITEM_SELECTION_STYLE_GRAY:
			style = LIST_VIEW_ITEM_SELECTION_STYLE_GRAY;
			break;
		case MAW_LIST_VIEW_ITEM_SELECTION_STYLE_NONE:
			style = LIST_VIEW_ITEM_SELECTION_STYLE_NONE;
			break;
		case MAW_LIST_VIEW_ITEM_SELECTION_STYLE_BLUE:
		default:
			style = LIST_VIEW_ITEM_SELECTION_STYLE_BLUE;
			break;
		}

		return style;
	}

	/**
	 * Convert enum to one of the
	 * \link MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE
	 * MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
	 * @param type Enum to convert.
	 * @return The converted value.
	 */
	int ListViewItem::getAccessoryTypeFromEnum(ListViewItemAccessory type)
	{
		int accessoryTypeInt;
		switch(type)
		{
		case LIST_VIEW_ITEM_ACCESSORY_DISCLOSURE:
			accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE;
			break;
		case LIST_VIEW_ITEM_ACCESSORY_DETAIL:
			accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL;
			break;
		case LIST_VIEW_ITEM_ACCESSORY_CHECKMARK:
			accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK;
			break;
		case LIST_VIEW_ITEM_ACCESSORY_NONE:
		default:
			accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE;
			break;
		}
		return accessoryTypeInt;
	}

	/**
	 * Convert a string value to one of the ListViewItemAccessory values.
	 * @param accessoryTypeString One of the
	 * \link MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE
	 * MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE \endlink values.
	 * @return The converted value.
	 */
	ListViewItemAccessory ListViewItem::getAccessoryTypeFromString(
		MAUtil::String& accessoryTypeString)
	{
		int accessoryTypeInt = MAUtil::stringToInteger(accessoryTypeString);
		ListViewItemAccessory type;
		switch (accessoryTypeInt)
		{
		case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE:
			type = LIST_VIEW_ITEM_ACCESSORY_DISCLOSURE;
			break;
		case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL:
			type = LIST_VIEW_ITEM_ACCESSORY_DETAIL;
			break;
		case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK:
			type = LIST_VIEW_ITEM_ACCESSORY_CHECKMARK;
			break;
		case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE:
		default:
			type = LIST_VIEW_ITEM_ACCESSORY_NONE;
			break;
		}
		return type;
	}

} // namespace NativeUI
