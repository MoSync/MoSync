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
