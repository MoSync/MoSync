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
 * @file NavigationBar.h
 * @author Emma Tresanszki
 *
 * Class for navigation bar.
 */

#ifndef NATIVEUI_NAVIGATION_BAR_H_
#define NATIVEUI_NAVIGATION_BAR_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class NavigationBarListener;

    /**
     * A navigation bar is an iPhone specific widget that shows a nav bar
     * with an optional title and back button.
     */
    class NavigationBar : public Widget
    {
    public:
        /**
         * Constructor.
         */
        NavigationBar();

        /**
         * Destructor.
         */
        virtual ~NavigationBar();

        /**
         * Set the title.
         * @param title The new title for the navigation bar.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setTitle(const MAUtil::String& title);

        /*
         * Get the title.
         * @return The displayed title.
         */
        virtual MAUtil::String getTitle() const;

        /**
         * Set the text on the back button.
         * @param text The new text for the back button.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setBackButtonTitle(const MAUtil::String& text);

        /*
         * Get the text on the back button.
         * @return The text displayed ont he back button.
         */
        virtual MAUtil::String getBackButtonTitle() const;

        /**
         * Add an navigation bar event listener.
         * @param listener The listener that will receive navigation bar events.
         */
        virtual void addNavigationBarListener(NavigationBarListener* listener);

        /**
         * Remove the navigation bar listener.
         * @param listener The listener that receives navigation bar events.
         */
        virtual void removeNavigationBarListener(
            NavigationBarListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with navigation bar listeners.
         */
        MAUtil::Vector<NavigationBarListener*> mNavigationBarListeners;
    };

} // namespace NativeUI

#endif
