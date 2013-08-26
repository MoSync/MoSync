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
 * @file RadioButton.h
 * @author Emma
 *
 * \brief An instance of RadioButton is used to create a two-states button that can
 * be either checked or unchecked.
 * A radio button can only have a RadioGroup parent.
 * Checking one radio button that belongs to a radio group unchecks any previously
 * checked radio button within the same group.
 * Initially, all of the radio buttons are unchecked.
 * While it is not possible to uncheck a particular radio button, the radio group
 * can be cleared to remove the checked state.
 * A radio button is available only on Android.
 * For radio group events see RadioButtonListener.
 */

#ifndef NATIVEUI_RADIO_BUTTON_H_
#define NATIVEUI_RADIO_BUTTON_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class RadioButtonListener;

    /**
     * \brief An instance of RadioButton is used to create a two-states button that can
     * be either checked or unchecked.
     * A radio button can only have a RadioGroup parent. Add a radio button to a radio
     * group by calling radioGroup->addView(radioButton).
     * Checking one radio button that belongs to a radio group unchecks any previously
     * checked radio button within the same group.
     * Initially, all of the radio buttons are unchecked.
     * While it is not possible to uncheck a particular radio button, the radio group
     * can be cleared to remove the checked state.
     * A radio button is available only on Android.
     * For radio group events see RadioButtonListener.
     */
    class RadioButton : public Widget
    {
    public:
        /**
         * Constructor.
         */
		RadioButton();

        /**
         * Destructor.
         */
        virtual ~RadioButton();

        /**
         * Change the checked state of the view to the inverse of its current state.
         * If the radio button is already checked, this method will not toggle the radio button.
         */
        virtual void toggle();

        /**
         * Set the text to display.
         * @param text The given text.
         */
        virtual void setText(const MAUtil::String text);

        /**
         * Get the displayed text.
         * @return The displayed text.
         */
        virtual MAUtil::String getText();

        /**
         * Specify the text color of the radio button.
         * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
         *              red, green and blue components respectively.
         */
        virtual void setTextColor(int color);

        /**
         * Get the checked state of the radio button.
         * @return true if the radio button is checked, false otherwise.
         */
        virtual bool isChecked();

        /**
         * Add a radio button event listener.
         * @param listener The listener that will receive radio button events.
         */
        virtual void addRadioButtonListener(RadioButtonListener* listener);

        /**
         * Remove the radio button event listener.
         * @param listener The listener that receives radio button events.
         */
        virtual void removeRadioButtonListener(RadioButtonListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with radio group listeners.
         */
        MAUtil::Vector<RadioButtonListener*> mRadioButtonListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_RADIO_BUTTON_H_ */

/*! @} */
