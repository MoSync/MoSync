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
 * @file NumberPickerListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for NumberPicker events.
 */

#ifndef NATIVEUI_NUMBER_PICKER_LISTENER_H_
#define NATIVEUI_NUMBER_PICKER_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class NumberPicker;

    /**
     * \brief Listener for NumberPicker events.
     */
    class NumberPickerListener
    {
    public:
        /**
         * This method is called when the selected number was changed by the
         * user.
         * @param numberPicker The number picker object that generated the event.
         * @param numberPickerValue The new selected value.
         */
        virtual void numberPickerValueChanged(
            NumberPicker* numberPicker,
            const int numberPickerValue) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_NUMBER_PICKER_LISTENER_H_ */

/*! @} */
