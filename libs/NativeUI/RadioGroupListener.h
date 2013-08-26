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
 * @file RadioGroupListener.h
 * @author Emma
 *
 * \brief Listener for RadioGroup events.
 */

#ifndef NATIVEUI_RADIO_GROUP_LISTENER_H_
#define NATIVEUI_RADIO_GROUP_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class RadioGroup;

    /**
     * \brief Listener for RadioGroup events.
     */
    class RadioGroupListener
    {
    public:
        /**
         * This method is called when a radio button from a radio group is selected.
         * When the selection is cleared, index is -1 and radioButton is null.
         * @param radioGroup The radio group object that generated the event.
         * @param index The index of the selected radio button inside this group.
         * @param radioButton The radio button handle that generated the event.
         */
        virtual void radioButtonSelected(
            RadioGroup* radioGroup,
            int index,
            RadioButton* radioButton) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_RADIO_GROUP_LISTENER_H_ */

/*! @} */
