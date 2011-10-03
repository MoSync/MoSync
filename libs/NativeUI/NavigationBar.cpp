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
 * @file NavigationBar.cpp
 * @author Emma Tresanszki
 *
 * Class for navigation bar.
 */

#include "NavigationBar.h"
#include "NavigationBarListener.h"

namespace NativeUI
{

    /**
     * Constructor.
     */
    NavigationBar::NavigationBar() :
        Widget(MAW_NAV_BAR)
    {
    }

    /**
     * Destructor.
     */
    NavigationBar::~NavigationBar()
    {
        mNavigationBarListeners.clear();
    }

    /**
     * Set the title.
     * @param title The new title for the navigation bar.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_ERROR otherwise.
     */
    int NavigationBar::setTitle(const MAUtil::String& title)
    {
        return setProperty(MAW_NAV_BAR_TITLE, title);
    }

    /*
     * Get the title.
     * @return The displayed title.
     */
    MAUtil::String NavigationBar::getTitle()
    {
        return getPropertyString(MAW_NAV_BAR_TITLE);
    }

    /**
     * Set the icon that will be displayed.
     * @param image MoSync handle to an uncompressed image resource.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
     */
    int NavigationBar::setIcon(MAHandle image)
    {
		return setPropertyInt(MAW_NAV_BAR_ICON, image);
    }

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
    int NavigationBar::setBackButtonTitle(const MAUtil::String& text)
    {
        return setProperty(MAW_NAV_BAR_BACK_BTN, text);
    }

    /*
     * Get the text on the back button.
     * Note: this is available only on iOS.
     * On Android the back behavior is handled by the device's back button.
     * @return The text displayed on the back button.
     */
    MAUtil::String NavigationBar::getBackButtonTitle()
    {
        return getPropertyString(MAW_NAV_BAR_BACK_BTN);
    }

    /**
     * Set the font color of the title.
     * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
     *              red, green and blue components respectively.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
     */
    int NavigationBar::setTitleFontColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        return this->setProperty(MAW_NAV_BAR_TITLE_FONT_COLOR, buffer);
    }

    /**
     * Set the font size in points of the title.
     * @param size A float that specifies the number of points.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
     */
    int NavigationBar::setTitleFontSize(const float size)
    {
		return this->setPropertyFloat(MAW_NAV_BAR_TITLE_FONT_SIZE, size);
    }

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
    int NavigationBar::setTitleFont(const MAHandle fontHandle)
    {
		return this->setPropertyInt(MAW_NAV_BAR_TITLE_FONT_HANDLE, fontHandle);
    }

    /**
     * Add an navigation bar event listener.
     * @param listener The listener that will receive navigation bar events.
     */
    void NavigationBar::addNavigationBarListener(NavigationBarListener* listener)
    {
        addListenerToVector(mNavigationBarListeners, listener);
    }

    /**
     * Remove the navigation bar listener.
     * @param listener The listener that receives navigation bar events.
     */
    void NavigationBar::removeNavigationBarListener(
        NavigationBarListener* listener)
    {
        removeListenerFromVector(mNavigationBarListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void NavigationBar::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_CLICKED == widgetEventData->eventType)
        {
            for (int i = 0; i < mNavigationBarListeners.size(); i++)
            {
                mNavigationBarListeners[i]->navigationBarBackButtonClicked(this);
            }
        }
    }

} // namespace NativeUI
