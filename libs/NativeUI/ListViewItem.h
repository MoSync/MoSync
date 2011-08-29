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
 * @file ListViewItem.h
 * @author Bogdan Iusco
 *
 * The ListViewItem class defines the attributes and behavior of the items
 * that appear in ListView objects.
 */

#ifndef NATIVEUI_LIST_VIEW_ITEM_H_
#define NATIVEUI_LIST_VIEW_ITEM_H_

#include "Widget.h"

namespace NativeUI
{

    /**
     * The ListViewItem class defines the attributes and behavior of the items
     * that appear in ListView objects.
     */
    enum ListViewItemAccessory
    {
        LIST_VIEW_ITEM_ACCESSORY_NONE = 0,
        LIST_VIEW_ITEM_ACCESSORY_DISCLOSURE,
        LIST_VIEW_ITEM_ACCESSORY_DETAIL,
        LIST_VIEW_ITEM_ACCESSORY_CHECKMARK
    };

    /**
     * Class for list view items.
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
         * Set an icon of the list view item that is placed to the left
         * of the text.
         * @param image MoSync handle to an uncompressed image resource.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the icon handle was invalid.
         */
        virtual int setIcon(MAHandle icon);

        /**
         * Set the type of accessory.
         * Provides the corresponding icon in the right side of the widget.
         * @param type The type of the accessory.
         */
        virtual void setAccessoryType(ListViewItemAccessory type);

        /**
        * Set the typeface and style in which the text should be displayed.
        * @param fontHandle A font handle received from loading fonts using
        *  #maFontGetName and #maFontLoadWithName syscalls.
        * @return Any of the following result codes:
        * - #MAW_RES_OK if the property could be set.
        * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
        * - #MAW_RES_ERROR otherwise.
        */
        virtual int setFont(const MAHandle fontHandle);
    };

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_ITEM_H_ */
