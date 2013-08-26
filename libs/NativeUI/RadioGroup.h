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
 * @file RadioGroup.h
 * @author Emma
 *
 * \brief An instance of RadioGroup is used to create a multiple-exclusion scope
 * for a set of RadioButtons.
 * A radio group is available only on Android.
 * For radio group events see RadioGroupListener.
 */

#ifndef NATIVEUI_RADIO_GROUP_H_
#define NATIVEUI_RADIO_GROUP_H_

#include "Widget.h"
#include "RadioButton.h"

namespace NativeUI
{

    // Forward declaration.
    class RadioGroupListener;

    /**
     * \brief An instance of RadioGroup is used to create a multiple-exclusion scope
     * for a set of RadioButtons.
     * A radio group is available only on Android.
     * For radio group events see RadioGroupListener.
     */
    class RadioGroup : public Widget
    {
    public:
        /**
         * Constructor.
         */
		RadioGroup();

        /**
         * Destructor.
         */
        virtual ~RadioGroup();

        /**
         *  Add the radio buttons grouped in this widget.
         *  This is the only widget type that accepts Radio Buttons as children.
         *  @param radioButton The child widget.
         */
        virtual void addView(RadioButton* radioButton);

        /**
         * Clear the selection.
         * When the selection is cleared, no radio button in this group is selected and
         * getChecked() returns -1.
         */
        virtual void clearCheck();

        /**
         * Sets the selected radio button in this group.
         * Setting it to NULL clears the selection. Such an operation
         * is equivalent to invoking clearCheck().
         * @param button The radio button object.
         * @return True if successful, false if the button is not in this group.
         */
        virtual bool setChecked(RadioButton* button);

        /**
         * Get the selected radio button in this group.
         * Upon empty selection, it returns NULL.
         * @return The checked radio button.
         */
        virtual RadioButton* getChecked();

        /**
         * Add a radio group event listener.
         * @param listener The listener that will receive radio group events.
         */
        virtual void addRadioGroupListener(RadioGroupListener* listener);

        /**
         * Remove the radio group event listener.
         * @param listener The listener that receives radio group events.
         */
        virtual void removeRadioGroupListener(RadioGroupListener* listener);

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
        MAUtil::Vector<RadioGroupListener*> mRadioGroupListeners;
    protected:
        /**
         * List of child radio buttons.
         */
        MAUtil::Vector<RadioButton*> mRadioButtons;
    };

} // namespace NativeUI

#endif /* NATIVEUI_RADIO_GROUP_H_ */

/*! @} */
