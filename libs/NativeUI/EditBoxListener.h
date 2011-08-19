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
 * @file EditBoxListener.h
 * @author Bogdan Iusco
 *
 * Listener for EditBox events.
 */

#ifndef NATIVEUI_EDIT_BOX_LISTENER_H_
#define NATIVEUI_EDIT_BOX_LISTENER_H_

namespace NativeUI
{

    // Forward declaration.
    class EditBox;

    /**
     * Listener for EditBox events.
     */
    class EditBoxListener
    {
    public:
        /**
         * This method is called when an edit box gains focus.
         * The virtual keyboard is shown.
         * Only for iphone platform.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxEditingDidBegin(EditBox* editBox) = 0;

        /**
         * This method is called when an edit box loses focus.
         * The virtual keyboard is hidden.
         * Only for iphone platform.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxEditingDidEnd(EditBox* editBox) = 0;

        /**
         * This method is called when the edit box text was changed.
         * Only for iphone platform.
         * @param editBox The edit box object that generated the event.
         * @param text The new text.
         */
        virtual void editBoxTextChanged(
            EditBox* editBox,
            const MAUtil::String& text) = 0;

        /**
         * This method is called when the return button was pressed.
         * On iphone platform the virtual keyboard is not hidden after
         * receiving this event.
         * Only for iphone platform.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxReturn(EditBox* editBox) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_EDIT_BOX_LISTENER_H_ */
