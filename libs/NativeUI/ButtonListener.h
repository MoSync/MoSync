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
 * @file ButtonListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for button events.
 *
 */

#ifndef NATIVEUI_BUTTON_LISTENER_H_
#define NATIVEUI_BUTTON_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class Widget;

    /**
     * \brief Listener for button events.
     */
    class ButtonListener
    {
    public:
        /**
         * This method is called when there is an touch-down event for
         * a button.
         * Platform: iOS and Android.
         * @param button The button object that generated the event.
         */
        virtual void buttonPressed(Widget* button) {};

        /**
         * This method is called when there is an touch-up event for
         * a button.
         * Platform: iOS and Android.
         * @param button The button object that generated the event.
         */
        virtual void buttonReleased(Widget* button) {};

        /**
         * This method is called if the touch-up event was inside the
         * bounds of the button.
         * Platform: iOS, Android, Windows Phone.
         * @param button The button object that generated the event.
         */
        virtual void buttonClicked(Widget* button) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_BUTTON_LISTENER_H_ */

/*! @} */
