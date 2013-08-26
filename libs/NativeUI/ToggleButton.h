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
 * @file ToggleButton.h
 * @author Emma Tresanszki
 *
 * \brief Class for a a special kind of check button.
 * It is available on Android, iOS and Windows Phone 7.
 * On iOS it looks and acts exactly the same as CheckBox widget.
 * For toggle button events see ToggleButtonListener
 */

#ifndef NATIVEUI_TOGGLE_BUTTON_H_
#define NATIVEUI_TOGGLE_BUTTON_H_

#include "Widget.h"

namespace NativeUI
{
	// Forward declaration.
	class ToggleButtonListener;

	/**
	 * \brief Class for a a special kind of check button.
	 * It is available on Android, iOS and Windows Phone 7.
	 * On iOS it looks and acts exactly the same as CheckBox widget.
	 * For toggle button events see ToggleButtonListener
	 *
	 * Displays checked/unchecked states as a button with a "light" indicator
	 * and by default accompanied with the text "ON" or "OFF".
	 */
	class ToggleButton : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		ToggleButton();

		/**
		 * Destructor.
		 */
		virtual ~ToggleButton();

		/*
		 * Sets the current toggle button state.
		 * @param toggleState The current state.
		 */
		virtual void setCheckedState(const bool toggleState);

		/*
		 * Gets the current toggle button state.
		 * @return The toggle button state.
		 */
		virtual bool isChecked();

        /**
         * Add a toggle button event listener.
         * @param listener The listener that will receive toggle button events.
         */
        virtual void addToggleButtonListener(ToggleButtonListener* listener);

        /**
         * Remove the toggle button event listener.
         * @param listener The listener that receives toggle button events.
         */
        virtual void removeToggleButtonListener(ToggleButtonListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with toggle button listeners.
         */
        MAUtil::Vector<ToggleButtonListener*> mToggleButtonListeners;
	};

} // namespace NativeUI

#endif

/*! @} */
