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
    int TimePicker::getHour() const
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
    int TimePicker::getMinute() const
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
