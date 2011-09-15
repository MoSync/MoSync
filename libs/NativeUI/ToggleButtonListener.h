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
 * @file ToggleButtonListener.h
 * @author Emma Tresanszki
 *
 * \brief Listener for ToggleButton events.
 */

#ifndef NATIVEUI_TOGGLE_BUTTON_LISTENER_H_
#define NATIVEUI_TOGGLE_BUTTON_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class ToggleButton;

    /**
     * \brief Listener for ToggleButton events.
     */
    class ToggleButtonListener
    {
    public:
        /**
         * This method is called when the state of the toggle button was changed
         * by the user.
         * @param toggleButton The toggle button object that generated the event.
         * @param state True if the toggle button is checked, false otherwise.
         */
        virtual void toggleButtonStateChanged(
            ToggleButton* toggleButton,
            bool state) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_TOGGLE_BUTTON_LISTENER_H_ */
