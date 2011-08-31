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
 * @file NumberPicker.h
 * @author Emma Tresanszki
 *
 * Class for number picker.
 * NOTE: This widget is available for Android only from level 11.
 */

#ifndef NATIVEUI_NUMBER_PICKER_H_
#define NATIVEUI_NUMBER_PICKER_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class NumberPickerListener;

	/**
	 * A Number Picker is a widget that enables the user to select a number
	 * from a predefined range.
	 * Note that this type of widget is available on Android since API level 11,
	 * so instantiating it on an older device will return a
	 * MAW_RES_FEATURE_NOT_AVAILABLE error code.
	 */
	class NumberPicker : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		NumberPicker();

		/**
		 * Destructor.
		 */
		virtual ~NumberPicker();

		/**
		 * Set the current value for the number picker.
		 * If the value is less than MAW_NUMBER_PICKER_MIN_VALUE property value,
		 * the current value is set to min.
		 * If the value is greater than MAW_NUMBER_PICKER_MAX_VALUE value,
		 * the current value is set to max.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setValue(const int value);

		/**
		 * Get the current value for the number picker.
		 * @return An int that specifies the picker value.
		 */
		virtual int getValue() const;

		/**
		 * Set the minimum value of the picker.
		 * @param minute An int that specifies the minimum value.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setMinValue(const int min);

		/**
		 * Get the minimum value of the picker.
		 * @return An int that specifies the minimum value.
		 */
		virtual int getMinValue() const;

		/**
		 * Set the maximum value of the picker.
		 * @param minute An int that specifies the maximum value.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setMaxValue(const int max);

		/**
		 * Get the maximum value of the picker.
		 * @return An int that specifies the maximum value.
		 */
		virtual int getMaxValue() const;

        /**
         * Add an number picker event listener.
         * @param listener The listener that will receive number picker events.
         */
        virtual void addNumberPickerListener(NumberPickerListener* listener);

        /**
         * Remove the number picker listener.
         * @param listener The listener that receives number picker events.
         */
        virtual void removeNumberPickerListener(NumberPickerListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);
    private:
        /**
         * Array with number picker listeners.
         */
        MAUtil::Vector<NumberPickerListener*> mNumberPickerListeners;
	};

} // namespace NativeUI

#endif
