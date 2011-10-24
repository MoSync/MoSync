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
 * @file ListViewListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for ListView events.
 */

#ifndef NATIVEUI_LIST_VIEW_LISTENER_H_
#define NATIVEUI_LIST_VIEW_LISTENER_H_

namespace NativeUI
{

    // Forward declaration.
    class ListView;
    class ListViewItem;

    /**
     * \brief Listener for ListView events.
     */
    class ListViewListener
    {
    public:
        /**
         * This method is called when a list view item is clicked.
         * @param listView The list view object that generated the event.
         * @param listViewItem The ListViewItem object that was clicked.
         */
        virtual void listViewItemClicked(
            ListView* listView,
            ListViewItem* listViewItem) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_LISTENER_H_ */

/*! @} */
