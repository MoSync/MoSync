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
 * @file SearchBar.h
 * @author Emma Tresanszki
 *
 * \brief Class for a special kind of edit box that is used for searching.
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
