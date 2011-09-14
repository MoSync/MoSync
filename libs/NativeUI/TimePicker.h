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

#ifndef NATIVEUI_TIME_PICKER_H_
#define NATIVEUI_TIME_PICKER_H_

#include "Widget.h"

namespace NativeUI
{
    // Forward declaration.
    class TimePickerListener;

    /**
     * A Time Picker is a widget for selecting time of day in 24 hour mode.
     * The time picker is initialized with system time.
     *
     * The hour and each minute digit can be controlled by vertical spinners.
     * The hour can be entered by keyboard input.
     */
    class TimePicker : public Widget
    {
    public:
        /**
         * Constructor.
         */
        TimePicker();

        /**
         * Destructor.
         */
        virtual ~TimePicker();

        /**
         * Set the current hour in 24h mode( in the range: 0-23 ).
         * @param hour An int that specifies the current hour.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setHour(const int hour);

        /**
         * Get the current hour in 24h mode( in the range: 0-23 ).
         * @return An int that specifies the current hour.
         */
        virtual int getHour();

        /**
         * Set the current minute (0-59).
         * @param minute An int that specifies the current minute.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setMinute(const int minute);

        /**
         * Get the current minute (0-59).
         * @return An int that specifies the current minute.
         */
        virtual int getMinute();

        /**
         * Add an time picker event listener.
         * @param listener The listener that will receive time picker events.
         */
        virtual void addTimePickerListener(TimePickerListener* listener);

        /**
         * Remove the time picker listener.
         * @param listener The listener that receives time picker events.
         */
        virtual void removeTimePickerListener(TimePickerListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with time picker listeners.
         */
        MAUtil::Vector<TimePickerListener*> mTimePickerListeners;
    };

} // namespace NativeUI

#endif
