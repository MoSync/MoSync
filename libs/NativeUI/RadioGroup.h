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
