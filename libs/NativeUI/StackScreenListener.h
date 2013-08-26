/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
