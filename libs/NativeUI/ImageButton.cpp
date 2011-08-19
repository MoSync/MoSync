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
 * @file ImageButton.cpp
 * @author Bogdan Iusco
 *
 * Class for image buttons.
 * A image button intercepts touch events and notifies the observers.
 * Methods for setting the text, alignment, font size and color are inherited
 * from TextWidget.
 */

#include "ImageButton.h"
#include "ButtonListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    ImageButton::ImageButton() : TextWidget(MAW_IMAGE_BUTTON)
    {
    }

    /**
     * Destructor.
     */
    ImageButton::~ImageButton()
    {
        mButtonListeners.clear();
    }

    /**
     * Set the foreground image of the button. This won't be scaled at all.
     * @param image MoSync handle to an uncompressed image resource.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
     */
    int ImageButton::setImage(MAHandle image)
    {
        return setPropertyInt(MAW_IMAGE_BUTTON_IMAGE, image);
    }

    /**
     * Sets the background image. This will be scaled to fit the whole
     * widget (not keeping the aspect).
     * @param image MoSync handle to an uncompressed image resource.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
     */
    int ImageButton::setBackgroundImage(MAHandle image)
    {
        return setPropertyInt(MAW_IMAGE_BUTTON_BACKGROUND_IMAGE, image);
    }

    /**
     * Add an button event listener.
     * @param listener The listener that will receive button events.
     */
    void ImageButton::addButtonListener(ButtonListener* listener)
    {
        addListenerToVector(mButtonListeners, listener);
    }

    /**
     * Remove the button listener.
     * @param listener The listener that receives button events.
     */
    void ImageButton::removeButtonListener(ButtonListener* listener)
    {
        removeListenerFromVector(mButtonListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void ImageButton::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_POINTER_PRESSED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonPressed(this);
            }
        }
        else if (MAW_EVENT_POINTER_RELEASED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonReleased(this);
            }
        }
        else if (MAW_EVENT_CLICKED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonClicked(this);
            }
        }
    }

} // namespace NativeUI
