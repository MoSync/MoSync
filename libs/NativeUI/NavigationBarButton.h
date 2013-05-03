/*
 The MIT License (MIT)

 Copyright (c) 2013 Gregor Gullwi

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file NavigationBarButton.h
 * @author Gregor Gullwi
 *
 * \brief Class for navigation bar buttons.
 */

#ifndef NATIVEUI_NAVIGATION_BAR_BUTTON_H_
#define NATIVEUI_NAVIGATION_BAR_BUTTON_H_

#include "Widget.h"

namespace NativeUI
{
    // Forward declaration.
    class ButtonListener;

    enum NavigationBarButtonType
    {
        NAV_BAR_BUTTON_TYPE_DONE,
        NAV_BAR_BUTTON_TYPE_CANCEL,
        NAV_BAR_BUTTON_TYPE_EDIT,
        NAV_BAR_BUTTON_TYPE_SAVE,
        NAV_BAR_BUTTON_TYPE_ADD,
        NAV_BAR_BUTTON_TYPE_FLEXIBLE_SPACE,
        NAV_BAR_BUTTON_TYPE_FIXED_SPACE,
        NAV_BAR_BUTTON_TYPE_COMPOSE,
        NAV_BAR_BUTTON_TYPE_REPLY,
        NAV_BAR_BUTTON_TYPE_ACTION,
        NAV_BAR_BUTTON_TYPE_ORGANIZE,
        NAV_BAR_BUTTON_TYPE_BOOKMARKS,
        NAV_BAR_BUTTON_TYPE_SEARCH,
        NAV_BAR_BUTTON_TYPE_REFRESH,
        NAV_BAR_BUTTON_TYPE_STOP,
        NAV_BAR_BUTTON_TYPE_CAMERA,
        NAV_BAR_BUTTON_TYPE_TRASH,
        NAV_BAR_BUTTON_TYPE_PLAY,
        NAV_BAR_BUTTON_TYPE_PAUSE,
        NAV_BAR_BUTTON_TYPE_REWIND,
        NAV_BAR_BUTTON_TYPE_FAST_FORWARD,
        NAV_BAR_BUTTON_TYPE_UNDO,
        NAV_BAR_BUTTON_TYPE_REDO,
        NAV_BAR_BUTTON_TYPE_PAGE_CURL,
        NAV_BAR_BUTTON_TYPE_WIDGET  = 0xFFFFFFFE,
        NAV_BAR_BUTTON_TYPE_NONE    = 0xFFFFFFFF
    };

    /**
     * \brief A navigation bar button is an iPhone specific widget that is a system or a text button attached to the navigation bar.
     * The default background color is light blue gradient, but can be changed by a tint color.
	 * Note: This widget is only available on iOS.
     *
     */
    class NavigationBarButton : public Widget
    {
    public:
        /**
         * Initializes the navigation bar button with a system icon
         * or with regular text if NAV_BAR_BUTTON_TYPE_NONE is used
         * @param buttonType Type or system button to use, or NAV_BAR_BUTTON_TYPE_NONE if it's text only
         */
        NavigationBarButton(NavigationBarButtonType buttonType);

        /**
         * Initializes the navigation bar button with an existing widget
         * @param widget Widget to show as the navigation bar button
         */
        NavigationBarButton(Widget* widget);

        virtual ~NavigationBarButton();

        /**
         * Set the text displayed by the widget.
         * @param text The given text.
         */
        virtual void setText(const MAUtil::String& text);

        /**
         * Get the text displayed by the widget.
         * @return The text displayed by the widget.
         */
        virtual MAUtil::String getText();

        /**
         * Set the background color of the widget.
         * @param color A hexadecimal color value in the ARGB format, e.g. 0xFFFF0000 is red.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the value was set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setTintColor(const int color);

        /**
         * Set the background color of the widget.
         * @param red Red component (range 0-255).
         * @param green Green component (range 0-255).
         * @param blue Blue component (range 0-255).
         * @param alpha Alpha component (range 0-255).
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the value was set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setTintColor(const int red, const int green, const int blue, const int alpha);

        /**
         * Add an button event listener.
         * @param listener The listener that will receive button events.
         */
        virtual void addButtonListener(ButtonListener* listener);

        /**
         * Remove the button listener.
         * @param listener The listener that receives button events.
         */
        virtual void removeButtonListener(ButtonListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with button listeners.
         */
        MAUtil::Vector<ButtonListener*> mButtonListeners;
    };

} // namespace NativeUI

#endif

/*! @} */
