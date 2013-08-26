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

/*! @} */
