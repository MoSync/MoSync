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
 * @file StackScreenListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for StackScreen events.
 *
 */

#ifndef NATIVEUI_STACK_SCREEN_LISTENER_H_
#define NATIVEUI_STACK_SCREEN_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class StackScreen;

    /**
     * \brief Listener for StackScreen events.
     */
    class StackScreenListener
    {
    public:
        /**
         * This method is called when a screen has been popped from a stack
         * screen.
         * @param stackScreen The stack screen object that generated the event.
         * @param fromScreen The screen that was popped from the stack screen.
         * @param toScreen The screen that will be shown.
         */
        virtual void stackScreenScreenPopped(
            StackScreen* stackScreen,
            Screen* fromScreen,
            Screen* toScreen) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_STACK_SCREEN_LISTENER_H_ */

/*! @} */
