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
