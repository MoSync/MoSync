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
 * \brief Listener for Dialog events.
 */

#ifndef NATIVEUI_DIALOG_LISTENER_H_
#define NATIVEUI_DIALOG_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class Dialog;

    /**
     * \brief Listener for Dialog events.
     */
    class DialogListener
    {
    public:

        /**
         * Note: only on Android.
         * This method is called when the right side button of the dialog
         * was clicked.
         * @param dialog The dialog object that generated the event.
         */
        virtual void dialogRightButtonClicked(Dialog* dialog);

        /**
         * Note: only on Android.
         * This method is called when the left side button of the dialog
         * was clicked.
         * @param dialog The dialog object that generated the event.
         */
        virtual void dialogLeftButtonClicked(Dialog* dialog);
    };

} // namespace NativeUI

#endif /* NATIVEUI_DIALOG_LISTENER_H_ */
