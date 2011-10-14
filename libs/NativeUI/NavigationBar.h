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
 * \brief Class for navigation bar.
 */

#ifndef NATIVEUI_NAVIGATION_BAR_H_
#define NATIVEUI_NAVIGATION_BAR_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class NavigationBarListener;

    /**
     * \brief A navigation bar is an iPhone specific widget that shows a nav bar
     * with an optional title, back button and icon.
     * The default background color is light blue gradient.
     * The appearance depends on the platform:
     * - on iOS it can have a back button on the left.
     * - on Android there's no extra button, the device's back button is used
     * instead.
     *
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
        virtual MAUtil::String getTitle();

        /**
         * Set the icon that will be displayed.
         * @param image MoSync handle to an uncompressed image resource.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
         */
        virtual int setIcon(MAHandle image);

        /**
         * Set the text on the back button.
         * Note: this is available only on iOS.
         * On Android the back behavior is handled by the device's back button.
         *
         * @param text The new text for the back button. If the value is an empty
         * string, then the button becomes invisible.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setBackButtonTitle(const MAUtil::String& text);

        /*
         * Get the text on the back button.
         * Note: this is available only on iOS.
         * On Android the back behavior is handled by the device's back button.
         * @return The text displayed on the back button.
         */
        virtual MAUtil::String getBackButtonTitle();

        /**
         * Set the font color of the title.
         * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
         *              red, green and blue components respectively.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
         */
        virtual int setTitleFontColor(const int color);

        /**
         * Set the font size in points of the title.
         * @param size A float that specifies the number of points.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
         */
        virtual int setTitleFontSize(const float size);

        /**
        * Set the typeface and style in which the title should be displayed.
        * @param fontHandle A font handle received from loading fonts using
        *  #maFontGetName and #maFontLoadWithName syscalls.
        * @return Any of the following result codes:
        * - #MAW_RES_OK if the property could be set.
        * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
        * - #MAW_RES_ERROR otherwise.
		* @par Example
		* \code
		*   int nrFonts = maFontGetCount();
		*    char buf[256];
		*    // Get first font name.
		*    maFontGetName(1, buf, 256);
		*    // Load that font for size 10.
		*    int fontHandle = maFontLoadWithName(buf, 10);
		*    // Set that font to the label.
		*	maWidgetSetProperty(
		*	    labelHandle, MAW_LABEL_FONT_HANDLE, toString(fontHandle));
		* \endcode
		*/
        virtual int setTitleFont(const MAHandle fontHandle);

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
