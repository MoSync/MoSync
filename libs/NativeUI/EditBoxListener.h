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
 * @file EditBoxListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for EditBox events.
 */

#ifndef NATIVEUI_EDIT_BOX_LISTENER_H_
#define NATIVEUI_EDIT_BOX_LISTENER_H_

namespace NativeUI
{

    // Forward declaration.
    class EditBox;

    /**
     * \brief Listener for EditBox events.
     */
    class EditBoxListener
    {
    public:
        /**
         * This method is called when an edit box gains focus.
         * The virtual keyboard is shown.
         * Platform: Android and iOS.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxEditingDidBegin(EditBox* editBox) {};

        /**
         * This method is called when an edit box loses focus.
         * The virtual keyboard is hidden.
         * Platform: Android and iOS.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxEditingDidEnd(EditBox* editBox) {};

        /**
         * This method is called when the edit box text was changed.
         * @param editBox The edit box object that generated the event.
         * @param text The new text.
         */
        virtual void editBoxTextChanged(
            EditBox* editBox,
            const MAUtil::String& text) {};

        /**
         * This method is called when the return button was pressed.
         * On iOS platform the virtual keyboard is not closed after
         * receiving this event.
         * This method is called only if the edit box mode is
         * #MAW_EDIT_BOX_SINGLE_LINE.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxReturn(EditBox* editBox) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_EDIT_BOX_LISTENER_H_ */

/*! @} */
