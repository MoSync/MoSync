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
