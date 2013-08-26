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
