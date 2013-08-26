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

/**
 * @file RatingBar.cpp
 * @author Emma
 *
 * \brief An instance of RatingBar is used for displaying a rating in stars.
 * The user can touch/drag or use arrow keys to set the rating when using
 * the default size RatingBar.
 * When using a RatingBar that supports user interaction, placing widgets to
 * the left or right of the RatingBar is discouraged.
 * For rating bar events see RatingBarListener.
 */

#include "RatingBar.h"
#include "RatingBarListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
	RatingBar::RatingBar(): Widget(MAW_RATING_BAR)
    {
    }

    /**
     * Destructor.
     */
	RatingBar::~RatingBar()
    {
        mRatingBarListeners.clear();
    }

    /**
     * Set the number of stars to show.
     * Usually, the default stars number is 5.
     * @param numStars The number of stars to show.
     */
    void RatingBar::setNumberOfStars(int numStars)
    {
		this->setPropertyInt(MAW_RATING_BAR_NUM_STAR, numStars);
    }

    /**
     * Get the number of shown stars.
     * @return The number of shown stars.
     */
    int RatingBar::getNumberOfStars()
    {
		return this->getPropertyInt(MAW_RATING_BAR_NUM_STAR);
    }

    /**
     * Set the current rating( number of stars filled).
     * @param rating The number of stars filled.
     */
    void RatingBar::setRating(float rating)
    {
		this->setPropertyFloat(MAW_RATING_BAR_RATING, rating);
    }

    /**
     * Get the current rating( number of stars filled).
     * @return The number of stars filled.
     */
    float RatingBar::getRating()
    {
		return this->getPropertyFloat(MAW_RATING_BAR_RATING);
    }

    /**
     * Set the step size ( granularity) of this rating bar.
     * By default the granularity is 0.5.
     * @param granularity The step size.
     */
    void RatingBar::setGranularity(float granularity)
    {
		this->setPropertyFloat(MAW_RATING_BAR_GRANULARITY, granularity);
    }

    /**
     * Get the step size ( granularity) of this rating bar.
     * @return The step size.
     */
    float RatingBar::getGranularity()
    {
		return this->getPropertyFloat(MAW_RATING_BAR_GRANULARITY);
    }

    /**
     * Add a rating bar event listener.
     * @param listener The listener that will receive rating bar events.
     */
    void RatingBar::addRatingBarListener(RatingBarListener* listener)
    {
		addListenerToVector(mRatingBarListeners, listener);
    }

    /**
     * Remove the rating bar listener.
     * @param listener The listener that receives rating bar events.
     */
    void RatingBar::removeRatingBarListener(RatingBarListener* listener)
    {
		removeListenerFromVector(mRatingBarListeners, listener);
    }


    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void RatingBar::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_RATING_BAR_VALUE_CHANGED == widgetEventData->eventType)
        {
			bool fromUser = widgetEventData->fromUser == 1 ? true : false;
			for (int i=0; i < mRatingBarListeners.size(); i++)
			{
				mRatingBarListeners[i]->ratingChanged(this, widgetEventData->value, fromUser);
			}
        }
    }

} // namespace NativeUI
