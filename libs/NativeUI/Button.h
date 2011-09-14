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
 * @file Button.h
 * @author Bogdan Iusco
 *
 * Class for buttons.
 * A button intercepts touch events and notifies the observers.
 * Methods for setting the text, alignment, font size and color are inherited
 * from TextWidget.
 */

#ifndef NATIVEUI_BUTTON_H_
#define NATIVEUI_BUTTON_H_

#include "TextWidget.h"

namespace NativeUI
{

    // Forward declaration.
    class ButtonListener;

    /**
     * Class for buttons.
     * A button intercepts touch events and notifies the observers.
     * Methods for setting the text, alignment, font size and color are
     * inherited from TextWidget.
     */
    class Button : public TextWidget
    {
    public:
        /**
         * Constructor.
         */
        Button();

        /**
         * Destructor.
         */
        virtual ~Button();

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

#endif /* NATIVEUI_BUTTON_H_ */
