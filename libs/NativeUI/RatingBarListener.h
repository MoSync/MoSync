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
 * @file RatingBarListener.h
 * @author Emma
 *
 * \brief Listener for RatingBar events.
 */

#ifndef NATIVEUI_RATING_BAR_LISTENER_H_
#define NATIVEUI_RATING_BAR_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class RatingBar;

    /**
     * \brief Listener for RatingBar events.
     */
    class RatingBarListener
    {
    public:
        /**
         * This method is called when the value of the rating bar was changed.
         * Clients can use the fromUser parameter to distinguish user-initiated
         * changes from those that occurred programmatically.
         * This will not be called continuously while the user is dragging, only
         * when the user finalizes a rating by lifting the touch.
         * @param ratingBar The rating bar object that generated the event.
         * @param value The new value of the rating bar.
         * @param fromUser True if the rating change was initiated by a user's
         * touch gesture or arrow key/horizontal trackbell movement.
         */
        virtual void ratingChanged(
            RatingBar* ratingBar,
            float value,
            bool fromUser) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_RATING_BAR_LISTENER_H_ */

/*! @} */
