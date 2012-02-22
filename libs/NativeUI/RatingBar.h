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
 * @file RatingBar.h
 * @author Emma
 *
 * \brief An instance of RatingBar is used for displaying a rating in stars.
 * The user can touch/drag or use arrow keys to set the rating when using
 * the default size RatingBar.
 * When using a RatingBar that supports user interaction, placing widgets to
 * the left or right of the RatingBar is discouraged.
 * For rating bar events see RatingBarListener.
 */

#ifndef NATIVEUI_RATING_BAR_H_
#define NATIVEUI_RATING_BAR_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class RatingBarListener;

    /**
     * \brief An instance of RatingBar is used for displaying a rating in stars.
     * Usually, the default stars number is 5.
     * The user can touch/drag or use arrow keys to set the rating when using
     * the default size RatingBar.
     * When using a RatingBar that supports user interaction, placing widgets to
     * the left or right of the RatingBar is discouraged.
     * For rating bar events see RatingBarListener.
     */
    class RatingBar : public Widget
    {
    public:
        /**
         * Constructor.
         */
		RatingBar();

        /**
         * Destructor.
         */
        virtual ~RatingBar();

        /**
         * Add a rating bar event listener.
         * @param listener The listener that will receive rating bar events.
         */
        virtual void addRatingBarListener(RatingBarListener* listener);

        /**
         * Remove the rating bar listener.
         * @param listener The listener that receives rating bar events.
         */
        virtual void removeRatingBarListener(RatingBarListener* listener);

        /**
         * Set the number of stars to show.
         * Usually, the default stars number is 5.
         * @param numStars The number of stars to show.
         */
        virtual void setNumberOfStars(int numStars);

        /**
         * Get the number of shown stars.
         * @return The number of shown stars.
         */
        virtual int getNumberOfStars();

        /**
         * Set the current rating( number of stars filled).
         * @param rating The number of stars filled.
         */
        virtual void setRating(float rating);

        /**
         * Get the current rating( number of stars filled).
         * @return The number of stars filled.
         */
        virtual float getRating();

        /**
         * Set the step size ( granularity) of this rating bar.
         * By default the granularity is 0.5.
         * @param granularity The step size.
         */
        virtual void setGranularity(float granularity);

        /**
         * Get the step size ( granularity) of this rating bar.
         * @return The step size.
         */
        virtual float getGranularity();

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with rating bar listeners.
         */
        MAUtil::Vector<RatingBarListener*> mRatingBarListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_RATING_BAR_H_ */

/*! @} */
