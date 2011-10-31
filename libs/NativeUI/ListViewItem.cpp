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
        MAUtil::String typeString;
        switch (type)
        {
            case LIST_VIEW_ITEM_ACCESSORY_NONE:
                typeString = "none";
                break;
            case LIST_VIEW_ITEM_ACCESSORY_DISCLOSURE:
                typeString = "hasChildren";
                break;
            case LIST_VIEW_ITEM_ACCESSORY_DETAIL:
                typeString = "hasDetails";
                break;
            case LIST_VIEW_ITEM_ACCESSORY_CHECKMARK:
                typeString = "isChecked";
                break;
        }

        this->setProperty(MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE, typeString);
    }

    /**
     * Set the font color of the item's text.
     * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
     *              red, green and blue components respectively.
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
     * @param size A float that specifies the number of points.
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
	*    char buf[256];
	*    // Get first font name.
	*    maFontGetName(1, buf, 256);
	*    // Load that font for size 10.
	*    int fontHandle = maFontLoadWithName(buf, 10);
	*    // Set that font to the label.
	*	maWidgetSetProperty(
	*	    labelHandle, MAW_LABEL_FONT_HANDLE, toString(fontHandle));
	* \endcode
	*/
    int ListViewItem::setFont(const MAHandle fontHandle)
    {
        return this->setPropertyInt(MAW_LIST_VIEW_ITEM_FONT_HANDLE, fontHandle);
    }

} // namespace NativeUI
