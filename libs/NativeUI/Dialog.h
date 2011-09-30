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
 * @file Dialog.h
 * @author emma
 *
 * \brief Class that represents a modal dialog.
 * A dialog cannot have any parent, it acts only as a container of other widgets.
 * A dialog is a sort of modal view, that can look different depending on
 * the platform:
 *  - On Android it is a modal alert dialog.
 *  - On iPad it is a PopoverController, and on iPhone it is a modal view.
 *
 * When a Dialog widget is created it is empty, it has no content.
 * Use setMainWidget(widget) to set the main widget of the dialog.
 *
 * A Dialog gets visible only after calling show() method.
 * To show a Dialog call show(), to hide it call: hide().
 *
 * On iPad there are two specific methods:
 * 	- setArrowPosition that sets the position of the popover origin arrow on
 * the screen.
 *  - canBeDismissed that allows or prohibits the user from dismissing a
 *  popover dialog by tapping outside of it.
 *
 * Note that on Android setting widget specific properties has effects only in
 * the dialog content(that is in fact the main widget), and not on the title bar.
 *
 * For dialog events see DialogListener.
 */

#ifndef NATIVEUI_DIALOG_H_
#define NATIVEUI_DIALOG_H_

#include "Widget.h"

namespace NativeUI
{
	// Forward declaration
	class DialogListener;

	/**
	 * \brief Class that represents a modal dialog.
	 */
	class Dialog : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		Dialog();

		/**
		 * Destructor.
		 */
		virtual ~Dialog();

		/**
		 * Set the title of the dialog.
		 * If the title is an empty string, the title bar becomes invisible.
		 * @param title The dialog title.
		 */
		virtual void setTitle(const MAUtil::String& title);

		/**
		 * Set the main widget of the dialog.
		 * Note: A dialog can only have one main widget.
		 * Use a layout as main widget and add child  widgets to the layout.
		 * @param widget The widget to be set as main widget.
		 * The ownership of the widget is passed to this function.
		 * When the dialog will be destroyed, the widget and it's child widgets
		 * will be deleted.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the child could be added to the parent.
		 * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
		 * - #MAW_RES_ERROR if it could not be added for some other reason.
		 */
		virtual int setMainWidget(Widget* widget);

		/**
		 * Sets the origin arrow position for a popover dialog.
		 * Note: This property is only available on the iPad.
		 * @param position one of the constants:
		 *  - #MAW_CONSTANT_ARROW_UP The popover arrow should point up.
		 *  - #MAW_CONSTANT_ARROW_DOWN The popover arrow should point down.
		 *  - #MAW_CONSTANT_ARROW_LEFT The popover arrow should point left.
		 *  - #MAW_CONSTANT_ARROW_RIGHT The popover arrow should point right.
		 *  - #MAW_CONSTANT_ARROW_ANY The popover arrow can point anywhere.
		 * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_NAME if the property is not available.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setArrrowPosition(const int position);

		/**
		 * Allow or prohibits the user from dismissing a popover dialog by
		 * tapping outside of it.
		 * Note: This property is only available on the iPad.
		 * @param state If true allows it to be dismissed, if false
		 * prohibits it.
		 * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_NAME if the property is not available.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
		 */
		virtual int canBeDismissed(bool state);

		/**
		 * Shows the dialog. Only one dialog at a time is visible.
		 * It will become visible on top of the UI.
		 */
		virtual void show();

		/**
		 * Hides a dialog if it is visible.
		 */
		virtual void hide();

        /**
         * Add an dialog event listener.
         * @param listener The listener that will receive dialog events.
         */
        virtual void addDialogListener(DialogListener* listener);

        /**
         * Remove the dialog listener.
         * @param listener The listener that receives dialog events.
         */
        virtual void removeDialogListener(DialogListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with dialog listeners.
         */
        MAUtil::Vector<DialogListener*> mDialogListeners;
	};

} // namespace NativeUI

#endif
