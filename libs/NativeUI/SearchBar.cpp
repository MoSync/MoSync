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

/**
 * @file SearchBar.cpp
 * @author Emma Tresanszki
 *
 * Class for a special kind of edit box that is used for searching.
 */

#include "SearchBar.h"
#include "SearchBarListener.h"

namespace NativeUI
{

    /**
     * Constructor.
     */
    SearchBar::SearchBar() :
        Widget(MAW_SEARCH_BAR)
    {
        // Set some common default values.
        this->fillSpaceHorizontally();
    }

    /**
     * Destructor.
     */
    SearchBar::~SearchBar()
    {
        mSearchBarListeners.clear();
    }

    /**
     * Set the current text of the search bar.
     * @param text The new text.
     */
    void SearchBar::setText(const MAUtil::String text)
    {
        this->setProperty(MAW_SEARCH_BAR_TEXT, text);
    }

    /**
     * Get the current text of the search bar.
     * @return The text.
     */
    MAUtil::String SearchBar::getText()
    {
        return this->getPropertyString(MAW_SEARCH_BAR_TEXT);
    }

    /**
     * Set a text in the search bar that acts as a placeholder
     * when an edit box is empty.
     * @param text The text that is displayed if the search bar is empty.
     */
    void SearchBar::setPlaceholder(const MAUtil::String text)
    {
        this->setProperty(MAW_SEARCH_BAR_PLACEHOLDER, text);
    }

    /**
     * Show the virtual keyboard.
     * The focus will be set to this widget.
     */
    void SearchBar::showKeyboard()
    {
        this->setProperty(MAW_SEARCH_BAR_SHOW_KEYBOARD, "true");
    }

    /**
     * Hide the virtual keyboard.
     */
    void SearchBar::hideKeyboard()
    {
        this->setProperty(MAW_SEARCH_BAR_SHOW_KEYBOARD, "false");
    }

    /**
     * Add an search bar event listener.
     * @param listener The listener that will receive search bar events.
     */
    void SearchBar::addSearchBarListener(SearchBarListener* listener)
    {
        addListenerToVector(mSearchBarListeners, listener);
    }

    /**
     * Remove the search bar listener.
     * @param listener The listener that receives search bar events.
     */
    void SearchBar::removeSearchBarListener(SearchBarListener* listener)
    {
        removeListenerFromVector(mSearchBarListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void SearchBar::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_CLICKED == widgetEventData->eventType)
        {
            int buttonClicked = widgetEventData->searchBarButton;

            for (int i = 0; i < mSearchBarListeners.size(); i++)
            {
                if (0 == buttonClicked)
                {
                    mSearchBarListeners[i]->searchBarSearchButtonClicked(this);
                }
                else if (1 == buttonClicked)
                {
                    mSearchBarListeners[i]->searchBarCancelButtonClicked(this);
                }
            }
        }
    }

} // namespace NativeUI
