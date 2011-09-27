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
 * @file ToggleButton.cpp
 * @author Emma Tresanszki
 *
 * Class for a a special kind of check button.
 * It is available only on Android, and it's similar to the Check Box on iOS.
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
