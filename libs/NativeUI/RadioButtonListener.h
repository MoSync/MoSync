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
