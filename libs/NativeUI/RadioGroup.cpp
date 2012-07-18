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
 * @file RadioGroup.
 * @author Emma
 *
 * \brief An instance of RadioGroup is used to create a multiple-exclusion scope
 * for a set of RadioButtons.
 * A radio group is available only on Android.
 * For radio group events see RadioGroupListener.
 */

#include "RadioGroup.h"
#include "RadioGroupListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
	RadioGroup::RadioGroup(): Widget(MAW_RADIO_GROUP)
    {
    }

    /**
     * Destructor.
     */
	RadioGroup::~RadioGroup()
    {
        mRadioGroupListeners.clear();
        for (int i = 0; i < mRadioButtons.size(); ++i)
        {
            delete mRadioButtons[i];
        }
        mRadioButtons.clear();
    }

    /**
     *  Add the radio buttons grouped in this widget.
     *  This is the only widget type that accepts Radio Buttons as children.
     *  @param radioButton The child widget.
     */
    void RadioGroup::addView(RadioButton* radioButton)
    {
		mRadioButtons.add(radioButton);
		setPropertyInt(MAW_RADIO_GROUP_ADD_VIEW, radioButton->getWidgetHandle());
    }

    /**
     * Clear the selection.
     * When the selection is cleared, no radio button in this group is selected and
     * getChecked() returns -1.
     */
    void RadioGroup::clearCheck()
    {
		setProperty(MAW_RADIO_GROUP_CLEAR_CHECK, "");
    }

    /**
     * Sets the selected radio button in this group.
     * Setting it to NULL clears the selection. Such an operation
     * is equivalent to invoking clearCheck().
     * @param button The radio button object.
     * @return True if successful, false if the button is not in this group.
     */
    bool RadioGroup::setChecked(RadioButton* button)
    {
		for (int i=0; i < mRadioButtons.size(); i++)
		{
			if ( mRadioButtons[i]->getWidgetHandle() == button->getWidgetHandle() )
			{
				setPropertyInt(MAW_RADIO_GROUP_SELECTED, button->getWidgetHandle());
				return true;
			}
		}
		return false;
    }

    /**
     * Get the selected radio button in this group.
     * Upon empty selection, it returns NULL.
     * @return The checked radio button.
     */
    RadioButton* RadioGroup::getChecked()
    {
		int buttonHandle = getPropertyInt(MAW_RADIO_GROUP_SELECTED);
		if ( buttonHandle == -1 )
			return NULL;
		for (int i=0; i < mRadioButtons.size(); i++)
		{
			if ( mRadioButtons[i]->getWidgetHandle() == buttonHandle )
			{
				return mRadioButtons[i];
			}
		}
		return NULL;
    }

    /**
     * Add a radio group event listener.
     * @param listener The listener that will receive radio group events.
     */
    void RadioGroup::addRadioGroupListener(RadioGroupListener* listener)
    {
		addListenerToVector(mRadioGroupListeners, listener);
    }

    /**
     * Remove the radio group event listener.
     * @param listener The listener that receives radio group events.
     */
    void RadioGroup::removeRadioGroupListener(RadioGroupListener* listener)
    {
		removeListenerFromVector(mRadioGroupListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void RadioGroup::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_RADIO_GROUP_ITEM_SELECTED == widgetEventData->eventType)
        {
			RadioButton* radioButton = NULL;
			for (int i=0; i < mRadioButtons.size(); i++)
			{
				if ( mRadioButtons[i]->getWidgetHandle() == widgetEventData->radioGroupItemHandle )
				{
					radioButton = mRadioButtons[i];
					break;
				}
			}
			for (int i=0; i < mRadioGroupListeners.size(); i++)
			{
				mRadioGroupListeners[i]->radioButtonSelected(this, widgetEventData->radioGroupItemHandle, radioButton);
			}
        }
    }

} // namespace NativeUI
