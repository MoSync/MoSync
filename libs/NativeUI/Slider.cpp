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
 * @file Slider.cpp
 * @author Bogdan Iusco
 *
 * An instance of Slider is used for selecting a single value from a
 * continuous range of values.
 */

#include "Slider.h"
#include "SliderListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    Slider::Slider(): Widget(MAW_SLIDER)
    {
    }

    /**
     * Destructor.
     */
    Slider::~Slider()
     {
        mSliderListeners.clear();
     }

    /**
     * Set the upper range of the slider.
     * @param maximumValue An positive integer that specifies the
     * upper range.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the maximumValue value
     *   was invalid.
     */
    int Slider::setMaximumValue(const int maximumValue)
    {
        return this->setPropertyInt(MAW_SLIDER_MAX, maximumValue);
    }

    /**
     * Get the upper range of the slider.
     * @return The upper range of the slider.
     */
    int Slider::getMaximumValue()
    {
        return this->getPropertyInt(MAW_SLIDER_MAX);
    }

    /**
     * Set the current value of the slider.
     * @param value An integer that specifies the new value,
     * between 0 and maximum.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the value was invalid.
     */
    int Slider::setValue(const int value)
    {
        return this->setPropertyInt(MAW_SLIDER_VALUE, value);
    }

    /**
     * Get the current value of the slider.
     * @return The slider's current value.
     */
    int Slider::getValue()
    {
        return this->getPropertyInt(MAW_SLIDER_VALUE);
    }

    /**
     * Increase the current value with a specified amount.
     * @param increaseValue An positive integer that specifies the amount
     * by which the slider's value should be increased.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the increaseValue value
     *   was invalid.
     */
    int Slider::increaseValue(const int value)
    {
        return this->setPropertyInt(MAW_SLIDER_INCREASE_VALUE, value);
    }

    /**
     * Decrease the current value with a specified amount.
     * @param decreaseValue An positive integer that specifies the amount
     * by which the slider's value should be decreased.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the decreaseValue value
     *   was invalid.
     */
    int Slider::decreaseValue(const int value)
    {
        return this->setPropertyInt(MAW_SLIDER_DECREASE_VALUE, value);
    }

    /**
     * Add an slider event listener.
     * @param listener The listener that will receive slider events.
     */
    void Slider::addSliderListener(SliderListener* listener)
    {
        addListenerToVector(mSliderListeners, listener);
    }

    /**
     * Remove the slider listener.
     * @param listener The listener that receives slider events.
     */
    void Slider::removeSliderListener(SliderListener* listener)
    {
        removeListenerFromVector(mSliderListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void Slider::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);
        int sliderValue = widgetEventData->sliderValue;

        if (MAW_EVENT_SLIDER_VALUE_CHANGED == widgetEventData->eventType)
        {
            for (int i = 0; i < mSliderListeners.size(); i++)
            {
                mSliderListeners[i]->sliderValueChanged(this, sliderValue);
            }
        }
    }

} // namespace NativeUI
