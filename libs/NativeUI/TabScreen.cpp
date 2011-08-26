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
 * @file TabScreen.cpp
 * @author Bogdan Iusco
 *
 * Class that represents a screen with tabs.
 * A tab screen is a special type of screen that can have a number of
 * sub-screens each switchable using a tab bar.
 */

#include "TabScreen.h"

#include "TabScreenListener.h"

namespace NativeUI
{

    /**
     * Constructor.
     */
    TabScreen::TabScreen() : Screen(MAW_TAB_SCREEN)
    {
    }

    /**
     * Destructor.
     */
     TabScreen::~TabScreen()
     {
         mTabScreenListeners.clear();
     }

    /**
     * Add a new tab with a screen in it.
     * @param screen The screen shown in the new tab.
     */
    void TabScreen::addTab(Screen* screen)
    {
        this->addChild(screen);
    }

    /**
     * Show a given tab.
     * @param index The index of the tab to show.
     * Index starts at zero.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_INDEX if the index was invalid.
     */
    int TabScreen::setActiveTab(const int index)
    {
        return (this->setPropertyInt(MAW_TAB_SCREEN_CURRENT_TAB, index));
    }

    /**
     * Returns the index of the current tab.
     * Index starts at zero.
     * @return The index of the current tab.
     */
    int TabScreen::getActiveTabIndex() const
    {
        return (this->getPropertyInt(MAW_TAB_SCREEN_CURRENT_TAB));
    }

    /**
     * Add an tab screen event listener.
     * @param listener The listener that will receive tab screen events.
     */
    void TabScreen::addTabScreenListener(TabScreenListener* listener)
    {
        addListenerToVector(
            mTabScreenListeners,
            listener);
    }

    /**
     * Remove the tab screen listener.
     * @param listener The listener that receives tab screen events.
     */
    void TabScreen::removeTabScreenListener(TabScreenListener* listener)
    {
        removeListenerFromVector(
            mTabScreenListeners,
            listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void TabScreen::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_TAB_CHANGED == widgetEventData->eventType)
        {
            int tabScreenIndex = widgetEventData->tabIndex;

            for (int i = 0; i < mTabScreenListeners.size(); i++)
            {
                mTabScreenListeners[i]->tabScreenTabChanged(
                    this,
                    tabScreenIndex);
            }
        }
    }

} // namespace NativeUI
