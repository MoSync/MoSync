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
 * @file CheckBox.h
 * @author Bogdan Iusco
 *
 * An instance of CheckBox is used for displaying an two-states button.
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
     * An instance of CheckBox is used for displaying an two-states button.
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
