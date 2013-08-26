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
 * @file Button.h
 * @author Bogdan Iusco
 *
 * \brief Class for buttons.
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
     * \brief Class for buttons.
     *
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

/*! @} */
