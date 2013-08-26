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
 * @file DatePickerListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for DatePicker events.
 */

#ifndef NATIVEUI_DATE_PICKER_LISTENER_H_
#define NATIVEUI_DATE_PICKER_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class DatePicker;
    struct Date;

    /**
     * \brief Listener for DatePicker events.
     */
    class DatePickerListener
    {
    public:
        /**
         * This method is called the selected date was changed by the user.
         * @param datePicker The date picker object that generated the event.
         * @param selectedDate The new selected date.
         */
        virtual void datePickerValueChanged(
            DatePicker* datePicker,
            const struct Date& selectedDate) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_DATE_PICKER_LISTENER_H_ */

/*! @} */
