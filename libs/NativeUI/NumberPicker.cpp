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
 * @file NumberPicker.cpp
 * @author Emma Tresanszki
 *
 * Class for number picker.
 * Note that this type of widget is available only on iOS for the moment.
 */

#include "NumberPicker.h"
#include "NumberPickerListener.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	NumberPicker::NumberPicker() :
		Widget(MAW_NUMBER_PICKER)
	{
	}

	/**
	 * Destructor.
	 */
	NumberPicker::~NumberPicker()
	{
	    mNumberPickerListeners.clear();
	}

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
	int NumberPicker::setValue(const int value)
	{
		return this->setPropertyInt(MAW_NUMBER_PICKER_VALUE, value);
	}

	/**
	 * Get the current value for the number picker.
	 * @return An int that specifies the picker value.
	 */
	int NumberPicker::getValue()
	{
		return this->getPropertyInt(MAW_NUMBER_PICKER_VALUE);
	}

	/**
	 * Set the minimum value of the picker.
	 * @param minute An int that specifies the minimum value.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int NumberPicker::setMinValue(const int min)
	{
		return this->setPropertyInt(MAW_NUMBER_PICKER_MIN_VALUE, min);
	}

	/**
	 * Get the minimum value of the picker.
	 * @return An int that specifies the minimum value.
	 */
	int NumberPicker::getMinValue()
	{
		return this->getPropertyInt(MAW_NUMBER_PICKER_MIN_VALUE);
	}

	/**
	 * Set the maximum value of the picker.
	 * @param minute An int that specifies the maximum value.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int NumberPicker::setMaxValue(const int max)
	{
		return this->setPropertyInt(MAW_NUMBER_PICKER_MAX_VALUE, max);
	}

	/**
	 * Get the maximum value of the picker.
	 * @return An int that specifies the maximum value.
	 */
	int NumberPicker::getMaxValue()
	{
		return this->getPropertyInt(MAW_NUMBER_PICKER_MAX_VALUE);
	}

    /**
     * Add an number picker event listener.
     * @param listener The listener that will receive number picker events.
     */
    void NumberPicker::addNumberPickerListener(NumberPickerListener* listener)
    {
        addListenerToVector(mNumberPickerListeners, listener);
    }

    /**
     * Remove the number picker listener.
     * @param listener The listener that receives number picker events.
     */
    void NumberPicker::removeNumberPickerListener(NumberPickerListener* listener)
    {
        removeListenerFromVector(mNumberPickerListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void NumberPicker::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED == widgetEventData->eventType)
        {
            int numberPickerValue = widgetEventData->numberPickerValue;
            for (int i = 0; i < mNumberPickerListeners.size(); i++)
            {
                mNumberPickerListeners[i]->numberPickerValueChanged(
                    this,
                    numberPickerValue);
            }
        }
    }

} // namespace NativeUI
