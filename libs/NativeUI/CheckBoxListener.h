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
 * @file CheckBoxListener.h
 * @author Bogdan Iusco
 *
 * Listener for CheckBox events.
 */

#ifndef NATIVEUI_CHECK_BOX_LISTENER_H_
#define NATIVEUI_CHECK_BOX_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class CheckBox;

    /**
     * Listener for CheckBox events.
     */
    class CheckBoxListener
    {
    public:
        /**
         * This method is called when the state of the check box was changed
         * by the user.
         * @param checkBox The check box object that generated the event.
         * @param state True if the check box is checked, false otherwise.
         */
        virtual void checkBoxStateChanged(
            CheckBox* checkBox,
            bool state) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_CHECK_BOX_LISTENER_H_ */
