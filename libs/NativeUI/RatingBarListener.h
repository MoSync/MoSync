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
