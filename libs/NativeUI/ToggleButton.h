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
