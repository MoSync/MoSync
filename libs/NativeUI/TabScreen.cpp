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
    int TabScreen::getActiveTabIndex()
    {
        return (this->getPropertyInt(MAW_TAB_SCREEN_CURRENT_TAB));
    }

	/**
	 * Check if a tab screen is shown.
	 * @return true if the tab screen is visible, false otherwise.
	 */
	bool TabScreen::isShown()
	{
		MAUtil::String value = this->getPropertyString(MAW_TAB_SCREEN_IS_SHOWN);
        return (strcmp(value.c_str(), "true") == 0) ? true : false;
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
        Screen::handleWidgetEvent(widgetEventData);

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
