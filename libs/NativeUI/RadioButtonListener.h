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
 * @file RadioButtonListener.h
 * @author Emma
 *
 * \brief Listener for RadioButton events.
 */

#ifndef NATIVEUI_RADIO_BUTTON_LISTENER_H_
#define NATIVEUI_RADIO_BUTTON_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class RadioButton;

    /**
     * \brief Listener for RadioButton events.
     */
    class RadioButtonListener
    {
    public:
        /**
         * This method is called when the checked state of a radio button is changed.
         * @param radioButton The radio button object that generated the event.
         * @param state The new state of the button.
         */
        virtual void radioButtonStateChanged(
            RadioButton* radioButton,
            bool state) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_RADIO_BUTTON_LISTENER_H_ */

/*! @} */
