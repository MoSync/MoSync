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
 * @file DialogListener.h
 * @author emma
 *
 * \brief Listener for modal dialog events.
 * Receives events only on the iPad.
 */

#ifndef NATIVEUI_DIALOG_LISTENER_H_
#define NATIVEUI_DIALOG_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class Dialog;

    /**
     * \brief Listener for modal dialog events.
     */
    class DialogListener
    {
    public:
        /**
         * Note: this is available only on the iPad.
         * This method is sent by a Dialog widget when when a user dismisses a
         * popover by tapping outside of it.
         * It will not be sent if it was dismissed by calling hide().
         * @param dialog The dialog object that generated the event.
         */
        virtual void dialogDismissed(Dialog* dialog) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_DIALOG_LISTENER_H_ */
