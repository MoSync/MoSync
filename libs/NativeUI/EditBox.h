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
 * @file EditBox.h
 * @author Bogdan Iusco
 *
 * Class for edit box.
 * You can use this widget to gather small amounts of text from the user.
 * The edit box supports the use of an observer to handle editing-related
 * events.
 */

#ifndef NATIVEUI_EDIT_BOX_H_
#define NATIVEUI_EDIT_BOX_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class EditBoxListener;

    /**
     * Types for edit box.
     */
    enum EditBoxMode
    {
        EDIT_BOX_MODE_TEXT = 0,
        EDIT_BOX_MODE_PASSWORD
    };

    /**
     * Class for edit box.
     * You can use this widget to gather small amounts of text from the user.
     * The edit box supports the use of an observer to handle editing-related
     * events.
     */
    class EditBox : public Widget
    {
    public:
        /**
         * Constructor.
         */
        EditBox();

        /**
         * Destructor.
         */
        virtual ~EditBox();

        /**
         * Set the text entered in the edit box.
         * @param text The given text.
         */
        virtual void setText(const MAUtil::String& text);

        /**
         * Get the text entered in the edit box.
         * @return The text entered in the edit box.
         */
        virtual MAUtil::String getText();

        /**
         * Set a text in the edit box that acts as a placeholder when an
         * edit box is empty.
         * @param The given text.
         */
        virtual void setPlaceholder(const MAUtil::String& text);

        /**
         * Show the virtual keyboard.
         * The focus will be set to this widget.
         */
        virtual void showKeyboard();

        /**
         * Hide the virtual keyboard.
         */
        virtual void hideKeyboard();

        /**
         * Set the mode of the edit box.
         * @param editBoxMode The given mode.
         */
        virtual void setEditMode(EditBoxMode editBoxMode);

        /**
         * Add an edit box event listener.
         * @param listener The listener that will receive edit box events.
         */
        virtual void addEditBoxListener(EditBoxListener* listener);

        /**
         * Remove the edit box listener.
         * @param listener The listener that receives edit box events.
         */
        virtual void removeEditBoxListener(EditBoxListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with edit box listeners.
         */
        MAUtil::Vector<EditBoxListener*> mEditBoxListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_EDITBOX_H_ */
