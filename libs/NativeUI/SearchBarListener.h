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
 * @file SearchBarListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for SearchBar events.
 *
 */

#ifndef NATIVEUI_SEARCH_BAR_LISTENER_H_
#define NATIVEUI_SEARCH_BAR_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class SearchBar;

    /**
     * \brief Interface for search bar event listeners.
     */
    class SearchBarListener
    {
    public:
        /**
         * This method is called when the search button is clicked.
         * @param searchBar The search bar object that generated the event.
         */
        virtual void searchBarSearchButtonClicked(SearchBar* searchBar) = 0;

        /**
         * This method is called when the cancel button is clicked.
         * @param searchBar The search bar object that generated the event.
         */
        virtual void searchBarCancelButtonClicked(SearchBar* searchBar) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_SEARCH_BAR_LISTENER_H_ */

/*! @} */
