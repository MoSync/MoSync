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
