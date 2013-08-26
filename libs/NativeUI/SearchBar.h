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
 * @file SearchBar.h
 * @author Emma Tresanszki
 *
 * \brief Class for a special kind of edit box that is used for searching.
 * Note: This is not available on Windows Phone 7.
 */

#ifndef NATIVEUI_SEARCH_BAR_H_
#define NATIVEUI_SEARCH_BAR_H_

#include "Widget.h"

namespace NativeUI
{
    // Forward declaration.
    class SearchBarListener;

    /**
     * \brief A search bar is a special kind of edit box that is used for
     * searching.
	 * Note: This is not available on Windows Phone 7.
     */
    class SearchBar : public Widget
    {
    public:
        /**
         * Constructor.
         */
        SearchBar();

        /**
         * Destructor.
         */
        virtual ~SearchBar();

        /**
         * Set the current text of the search bar.
         * @param text The new text.
         */
        virtual void setText(const MAUtil::String text);

        /**
         * Get the current text of the search bar.
         * @return The text.
         */
        virtual MAUtil::String getText();

        /**
         * Set a text in the search bar that acts as a placeholder
         * when an edit box is empty.
         * @param text The text that is displayed if the search bar is empty.
         */
        virtual void setPlaceholder(const MAUtil::String text);

        /**
         * Show the virtual keyboard.
         * The focus will be set to this widget.
         */
        virtual void showKeyboard();

        /**
         * Hide the virtual keyboard.
         */
        virtual void hideKeyboard();

        /**
         * Add an search bar event listener.
         * @param listener The listener that will receive search bar events.
         */
        virtual void addSearchBarListener(SearchBarListener* listener);

        /**
         * Remove the search bar listener.
         * @param listener The listener that receives search bar events.
         */
        virtual void removeSearchBarListener(SearchBarListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with search bar listeners.
         */
        MAUtil::Vector<SearchBarListener*> mSearchBarListeners;
    };

} // namespace NativeUI

#endif

/*! @} */
