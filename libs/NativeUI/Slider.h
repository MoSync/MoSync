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
 * @file Slider.h
 * @author Bogdan Iusco
 *
 * \brief An instance of Slider is used for selecting a single value from a
 * continuous range of values.
 */

#ifndef NATIVEUI_SLIDER_H_
#define NATIVEUI_SLIDER_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class SliderListener;

    /**
     * \brief An instance of Slider is used for selecting a single value from a
     * continuous range of values.
     */
    class Slider : public Widget
    {
    public:
        /**
         * Constructor.
         */
        Slider();

        /**
         * Destructor.
         */
        virtual ~Slider();

        /**
         * Set the upper range of the slider.
         * @param maximumValue An positive integer that specifies the
         * upper range.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the maximumValue value
         *   was invalid.
         */
        virtual int setMaximumValue(const int maximumValue);

        /**
         * Get the upper range of the slider.
         * @return The upper range of the slider.
         */
        virtual int getMaximumValue();

        /**
         * Set the current value of the slider.
         * @param value An integer that specifies the new value,
         * between 0 and maximum.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the value was invalid.
         */
        virtual int setValue(const int value);

        /**
         * Get the current value of the slider.
         * @return The slider's current value.
         */
        virtual int getValue();

        /**
         * Increase the current value with a specified amount.
         * @param increaseValue An positive integer that specifies the amount
         * by which the slider's value should be increased.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the increaseValue value
         *   was invalid.
         */
        virtual int increaseValue(const int increaseValue);

        /**
         * Decrease the current value with a specified amount.
         * @param decreaseValue An positive integer that specifies the amount
         * by which the slider's value should be decreased.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the decreaseValue value
         *   was invalid.
         */
        virtual int decreaseValue(const int decreaseValue);

        /**
         * Add an slider event listener.
         * @param listener The listener that will receive slider events.
         */
        virtual void addSliderListener(SliderListener* listener);

        /**
         * Remove the slider listener.
         * @param listener The listener that receives slider events.
         */
        virtual void removeSliderListener(SliderListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with slider listeners.
         */
        MAUtil::Vector<SliderListener*> mSliderListeners;
    };

} // namespace NativeUI

#endif /* SLIDER_H_ */

/*! @} */
