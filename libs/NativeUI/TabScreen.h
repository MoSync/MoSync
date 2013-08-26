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
		 * Check if a tab screen is shown.
		 * @return true if the tab screen is visible, false otherwise.
		 */
		virtual bool isShown();

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

/*! @} */
