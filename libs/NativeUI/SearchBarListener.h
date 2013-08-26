/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
