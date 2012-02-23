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
 * @file RadioButton.cpp
 * @author Emma
 *
 * \brief An instance of RadioButton is used to create a two-states button that can
 * be either checked or unchecked.
 * A radio button can only have a RadioGroup parent.
 * Checking one radio button that belongs to a radio group unchecks any previously
 * checked radio button within the same group.
 * Initially, all of the radio buttons are unchecked.
 * While it is not possible to uncheck a particular radio button, the radio group
 * can be cleared to remove the checked state.
 * A radio button is available only on Android.
 * For radio group events see RadioButtonListener.
 */

#include "RadioButton.h"
#include "RadioButtonListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
	RadioButton::RadioButton(): Widget(MAW_RADIO_BUTTON)
    {
    }

    /**
     * Destructor.
     */
	RadioButton::~RadioButton()
    {
        mRadioButtonListeners.clear();
    }

    /**
     * Change the checked state of the view to the inverse of its current state.
     * If the radio button is already checked, this method will not toggle the radio button.
     */
    void RadioButton::toggle()
    {
		setProperty(MAW_RADIO_BUTTON_TOGGLE, "");
    }

    /**
     * Set the text to display.
     * @param text The given text.
     */
    void RadioButton::setText(const MAUtil::String text)
    {
		setProperty(MAW_RADIO_BUTTON_TEXT, text);
    }

    /**
     * Get the displayed text.
     * @return The displayed text.
     */
    MAUtil::String RadioButton::getText()
    {
		return getPropertyString(MAW_RADIO_BUTTON_TEXT);
    }

    /**
     * Specify the text color of the radio button.
     * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
     *              red, green and blue components respectively.
     */
    void RadioButton::setTextColor(int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        setProperty(MAW_RADIO_BUTTON_TEXT_COLOR, buffer);
    }

    /**
     * Get the checked state of the radio button.
     * @return true if the radio button is checked, false otherwise.
     */
    bool RadioButton::isChecked()
    {
		MAUtil::String state = MAUtil::lowerString(getPropertyString(MAW_RADIO_BUTTON_CHECKED));
		return ( strcmp( state.c_str(),"true") == 0 ? true : false );
    }

    /**
     * Add a radio button event listener.
     * @param listener The listener that will receive radio button events.
     */
    void RadioButton::addRadioButtonListener(RadioButtonListener* listener)
    {
		addListenerToVector(mRadioButtonListeners, listener);
    }

    /**
     * Remove the radio button event listener.
     * @param listener The listener that receives radio button events.
     */
    void RadioButton::removeRadioButtonListener(RadioButtonListener* listener)
    {
		removeListenerFromVector(mRadioButtonListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void RadioButton::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if ( MAW_EVENT_RADIO_BUTTON_STATE_CHANGED == widgetEventData->eventType )
        {
			for (int i=0; i < mRadioButtonListeners.size(); i++)
			{
				mRadioButtonListeners[i]->radioButtonStateChanged(this, widgetEventData->radioButtonState);
			}
        }
    }

} // namespace NativeUI
