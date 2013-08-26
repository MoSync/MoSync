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
 * @file ToggleButton.cpp
 * @author Emma Tresanszki
 *
 * Class for a a special kind of check button.
 * It is available on Android, iOS and Windows Phone 7.
 * On iOS it looks and acts exactly the same as CheckBox widget.
 * For toggle button events see ToggleButtonListener
 */

#include "ToggleButton.h"
#include "ToggleButtonListener.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	ToggleButton::ToggleButton() :
		Widget(MAW_TOGGLE_BUTTON)
	{
	}

	/**
	 * Destructor.
	 */
	ToggleButton::~ToggleButton()
	{
		mToggleButtonListeners.clear();
	}

	/*
	 * Sets the current toggle button state.
	 * @param toggleState The current state.
	 */
	void ToggleButton::setCheckedState(const bool toggleState)
	{
		this->setProperty(
			MAW_TOGGLE_BUTTON_CHECKED, (toggleState ? "true" : "false") );
	}

	/*
	 * Gets the current toggle button state.
	 * @return The toggle button state.
	 */
	bool ToggleButton::isChecked()
	{
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MAW_TOGGLE_BUTTON_CHECKED));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
	}

	/**
	 * Add an toggle button event listener.
	 * @param listener The listener that will receive toggle button events.
	 */
	void ToggleButton::addToggleButtonListener(ToggleButtonListener* listener)
	{
		addListenerToVector(mToggleButtonListeners, listener);
	}

	/**
	 * Remove the toggle button listener.
	 * @param listener The listener that receives toggle button events.
	 */
	void ToggleButton::removeToggleButtonListener(ToggleButtonListener* listener)
	{
		removeListenerFromVector(mToggleButtonListeners, listener);
	}

	/**
	 * This method is called when there is an event for this widget.
	 * It passes on the event to all widget's listeners.
	 * @param widgetEventData The data for the widget event.
	 */
	void ToggleButton::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		Widget::handleWidgetEvent(widgetEventData);

		if ( MAW_EVENT_CLICKED == widgetEventData->eventType )
		{
			bool toggleBtnState = widgetEventData->checked == 1 ? true : false;
			for (int i = 0; i < mToggleButtonListeners.size(); i++)
			{
				mToggleButtonListeners[i]->toggleButtonStateChanged(
					this, toggleBtnState);
			}
		}
	}
} // namespace NativeUI
