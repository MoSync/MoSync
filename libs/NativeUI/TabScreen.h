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
 * @file TabScreen.h
 * @author Bogdan Iusco
 *
 * \brief Class that represents a screen with tabs.
 * A tab screen is a special type of screen that can have a number of
 * sub-screens each switchable using a tab bar.
 */

#ifndef NATIVEUI_TAB_SCREEN_H_
#define NATIVEUI_TAB_SCREEN_H_

#include "Screen.h"

namespace NativeUI
{

    // Forward declaration.
    class TabScreenListener;

    /**
     * \brief Class that represents a screen with tabs.
     *
     * A tab screen is a special type of screen that can have a number of
     * sub-screens each switchable using a tab bar.
     */
    class TabScreen : public Screen
    {
    public:
        /**
         * Constructor.
         */
        TabScreen();

        /**
         * Destructor.
         */
        virtual ~TabScreen();

        /**
         * Add a new tab with a screen in it.
         * @param screen The screen shown in the new tab.
         */
        virtual void addTab(Screen* screen);

        /**
         * Show a given tab.
         * @param index The index of the tab to show.
         * Index starts at zero.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_INDEX if the index was invalid.
         */
        virtual int setActiveTab(const int index);

        /**
         * Returns the index of the current tab.
         * Index starts at zero.
         * @return The index of the current tab.
         */
        virtual int getActiveTabIndex();

        /**
         * Add an tab screen event listener.
         * @param listener The listener that will receive tab screen events.
         */
        virtual void addTabScreenListener(TabScreenListener* listener);

        /**
         * Remove the tab screen listener.
         * @param listener The listener that receives tab screen events.
         */
        virtual void removeTabScreenListener(TabScreenListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with tab screen listeners.
         */
        MAUtil::Vector<TabScreenListener*> mTabScreenListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_TAB_SCREEN_H_ */
