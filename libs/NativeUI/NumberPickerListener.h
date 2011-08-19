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
 * @file NumberPickerListener.h
 * @author Bogdan Iusco
 *
 * Listener for NumberPicker events.
 */

#ifndef NATIVEUI_NUMBER_PICKER_LISTENER_H_
#define NATIVEUI_NUMBER_PICKER_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class NumberPicker;

    /**
     * Listener for NumberPicker events.
     */
    class NumberPickerListener
    {
    public:
        /**
         * This method is called when the selected number was changed by the
         * user.
         * @param numberPicker The number picker object that generated the event.
         * @param numberPickerValue The new selected value.
         */
        virtual void numberPickerValueChanged(
            NumberPicker* numberPicker,
            const int numberPickerValue) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_NUMBER_PICKER_LISTENER_H_ */
