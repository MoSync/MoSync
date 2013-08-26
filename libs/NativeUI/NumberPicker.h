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
 * @file NumberPicker.h
 * @author Emma Tresanszki
 *
 * \brief Class for number picker.
 * Available on iOS, WindowsPhone and Android.
 */

#ifndef NATIVEUI_NUMBER_PICKER_H_
#define NATIVEUI_NUMBER_PICKER_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class NumberPickerListener;

	/**
	 * \brief  A Number Picker is a widget that enables the user to select a
	 * number from a predefined range.
	 *
	 * Available on iOS, WindowsPhone and Android.
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
		 * @param value The value that we want to set.
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
		virtual int getValue();

		/**
		 * Set the minimum value of the picker.
		 * @param min An int that specifies the minimum value.
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
		virtual int getMinValue();

		/**
		 * Set the maximum value of the picker.
		 * @param max An int that specifies the maximum value.
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
		virtual int getMaxValue();

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

/*! @} */
