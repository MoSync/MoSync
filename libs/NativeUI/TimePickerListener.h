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
 * @file TimePickerListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for TimePicker events.
 */

#ifndef NATIVEUI_TIME_PICKER_LISTENER_H_
#define NATIVEUI_TIME_PICKER_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class TimePicker;

    /**
     * \brief Listener for TimePicker events.
     */
    class TimePickerListener
    {
    public:
        /**
         * This method is called when the selected time was changed by the user.
         * @param timePicker The time picker object that generated the event.
         * @param hour The selected hour.
         * @param minute The selected minute
         */
        virtual void timePickerValueChanged(
            TimePicker* timePicker,
            const int hour,
            const int minute) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_TIME_PICKER_LISTENER_H_ */

/*! @} */
