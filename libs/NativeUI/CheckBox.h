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
 * @file CheckBox.h
 * @author Bogdan Iusco
 *
 * \brief An instance of CheckBox is used for displaying an two-states button.
 * For check box events see CheckBoxListener.
 */

#ifndef NATIVEUI_CHECK_BOX_H_
#define NATIVEUI_CHECK_BOX_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class CheckBoxListener;

    /**
     * \brief An instance of CheckBox is used for displaying an two-states button.
     *
     * For check box events see CheckBoxListener.
     */
    class CheckBox : public Widget
    {
    public:
        /**
         * Constructor.
         */
        CheckBox();

        /**
         * Destructor.
         */
        virtual ~CheckBox();

        /**
         * Set the state of the check box.
         * @param state True if the check box should be checked, false otherwise.
         */
        virtual void setState(const bool state);

        /**
         * Get the state of the check box.
         * @return True if the check box is checked, false otherwise.
         */
        virtual bool isChecked();

        /**
         * Add an check box event listener.
         * @param listener The listener that will receive check box events.
         */
        virtual void addCheckBoxListener(CheckBoxListener* listener);

        /**
         * Remove the check box listener.
         * @param listener The listener that receives check box events.
         */
        virtual void removeCheckBoxListener(CheckBoxListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with check box listeners.
         */
        MAUtil::Vector<CheckBoxListener*> mCheckBoxListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_CHECK_BOX_H_ */

/*! @} */
