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

/**
 * @file TimePicker.h
 * @author Emma Tresanszki
 *
 * An instance of TimePicker allow to an users to select times.
 * For time picker events see TimePickerListener.
 */

#include "TimePicker.h"
#include "TimePickerListener.h"

namespace NativeUI
{

    /**
     * Constructor.
     */
    TimePicker::TimePicker() :
        Widget(MAW_TIME_PICKER)
    {
    }

    /**
     * Destructor.
     */
    TimePicker::~TimePicker()
    {
        mTimePickerListeners.clear();
    }

    /**
     * Set the current hour in 24h mode( in the range: 0-23 ).
     * @param hour An int that specifies the current hour.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int TimePicker::setHour(const int hour)
    {
        return this->setPropertyInt(MAW_TIME_PICKER_CURRENT_HOUR, hour);
    }

    /**
     * Get the current hour in 24h mode( in the range: 0-23 ).
     * @return An int that specifies the current hour.
     */
    int TimePicker::getHour()
    {
        return this->getPropertyInt(MAW_TIME_PICKER_CURRENT_HOUR);
    }

    /**
     * Set the current minute (0-59).
     * @param minute An int that specifies the current minute.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int TimePicker::setMinute(const int minute)
    {
        return this->setPropertyInt(MAW_TIME_PICKER_CURRENT_MINUTE, minute);
    }

    /**
     * Get the current minute (0-59).
     * @return An int that specifies the current minute.
     */
    int TimePicker::getMinute()
    {
        return this->getPropertyInt(MAW_TIME_PICKER_CURRENT_MINUTE);
    }

    /**
     * Add an time picker event listener.
     * @param listener The listener that will receive time picker events.
     */
    void TimePicker::addTimePickerListener(TimePickerListener* listener)
    {
        addListenerToVector(mTimePickerListeners, listener);
    }

    /**
     * Remove the time picker listener.
     * @param listener The listener that receives time picker events.
     */
    void TimePicker::removeTimePickerListener(TimePickerListener* listener)
    {
        removeListenerFromVector(mTimePickerListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void TimePicker::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_TIME_PICKER_VALUE_CHANGED == widgetEventData->eventType)
        {
            int selectedHour = widgetEventData->hour;
            int selectedMinute = widgetEventData->minute;

            for (int i = 0; i < mTimePickerListeners.size(); i++)
            {
                mTimePickerListeners[i]->timePickerValueChanged(
                    this,
                    selectedHour,
                    selectedMinute);
            }
        }
    }

} // namespace NativeUI
