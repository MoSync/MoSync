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
