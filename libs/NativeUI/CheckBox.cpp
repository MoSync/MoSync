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
 * @file CheckBox.cpp
 * @author Bogdan Iusco
 *
 * An instance of CheckBox is used for displaying an two-states button.
 * For check box events see CheckBoxListener.
 */

#include "CheckBox.h"
#include "CheckBoxListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    CheckBox::CheckBox(): Widget(MAW_CHECK_BOX)
    {
    }

    /**
     * Destructor.
     */
    CheckBox::~CheckBox()
    {
        mCheckBoxListeners.clear();
    }

    /**
     * Set the state of the check box.
     * @param state True if the check box should be checked, false otherwise.
     */
    void CheckBox::setState(const bool state)
    {
        MAUtil::String value = state ? "true" : "false";
        this->setProperty(MAW_CHECK_BOX_CHECKED, value);
    }

    /**
     * Get the state of the check box.
     * @return True if the check box is checked, false otherwise.
     */
    bool CheckBox::isChecked()
    {
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MAW_CHECK_BOX_CHECKED));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
    }

    /**
     * Add an check box event listener.
     * @param listener The listener that will receive check box events.
     */
    void CheckBox::addCheckBoxListener(CheckBoxListener* listener)
    {
       addListenerToVector(mCheckBoxListeners, listener);
    }

    /**
     * Remove the check box listener.
     * @param listener The listener that receives check box events.
     */
    void CheckBox::removeCheckBoxListener(CheckBoxListener* listener)
    {
        removeListenerFromVector(mCheckBoxListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void CheckBox::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_CLICKED == widgetEventData->eventType)
        {
            bool checkBoxState = widgetEventData->checked == 1 ? true : false;
            for (int i = 0; i < mCheckBoxListeners.size(); i++)
            {
                mCheckBoxListeners[i]->checkBoxStateChanged(this, checkBoxState);
            }
        }
    }

} // namespace NativeUI
