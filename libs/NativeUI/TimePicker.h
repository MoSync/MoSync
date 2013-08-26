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
 * @file TimePicker.h
 * @author Emma Tresanszki
 *
 * \brief An instance of TimePicker allow to an users to select times.
 * For time picker events see TimePickerListener.
 * Note: This is not available on Windows Phone 7.
 */

#ifndef NATIVEUI_TIME_PICKER_H_
#define NATIVEUI_TIME_PICKER_H_

#include "Widget.h"

namespace NativeUI
{
    // Forward declaration.
    class TimePickerListener;

    /**
     * \brief A Time Picker is a widget for selecting time of day in 24 hour mode.
     *
     * The time picker is initialized with system time.
     * The hour and each minute digit can be controlled by vertical spinners.
     * The hour can be entered by keyboard input.
     * Note: This is not available on Windows Phone 7.
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

/*! @} */
