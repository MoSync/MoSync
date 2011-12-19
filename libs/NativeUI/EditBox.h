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
 * @file EditBox.h
 * @author Bogdan Iusco
 *
 * \brief Class for edit box.
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
     * The EditBoxInputMode defines the type of text that the user is allowed to
     * enter.
     */
    enum EditBoxInputMode
    {
		/**
		* \brief The user is allowed to enter any text, including line breaks.
		*/
		EDIT_BOX_INPUT_MODE_ANY = 0,
		/**
		* The user is allowed to enter an e-mail address.
		*/
		EDIT_BOX_INPUT_MODE_EMAILADDR = 1,
		/**
		* The user is allowed to enter an integer value.
		*/
		EDIT_BOX_INPUT_MODE_NUMERIC = 2,
		/**
		* The user is allowed to enter a phone number.
		*/
		EDIT_BOX_INPUT_MODE_PHONENUMBER = 3,
		/**
		* The user is allowed to enter a URL.
		*/
		EDIT_BOX_INPUT_MODE_URL = 4,
		/**
		* The user is allowed to enter a real number value.
		* This extends EDIT_BOX_INPUT_MODE_NUMERIC by allowing a decimal point.
		*/
		EDIT_BOX_INPUT_MODE_DECIMAL = 5,
		/**
		* The user is allowed to enter any text, except for line breaks.
		*/
		EDIT_BOX_INPUT_MODE_SINGLE_LINE = 6
    };

    /**
     * The EditBoxInputFlag defines how the input text is displayed/formatted.
     */
    enum EditBoxInputFlag
    {
		/**
		* Indicates that the text entered is confidential data that should be
		* obscured whenever possible. This implies EDIT_BOX_INPUT_FLAG_SENSITIVE.
		*/
		EDIT_BOX_INPUT_FLAG_PASSWORD = 0,
		/**
		* Indicates that the text entered is sensitive data that the
		* implementation must never store into a dictionary or table for use
		* in predictive, auto-completing, or other accelerated input schemes.
		* A credit card number is an example of sensitive data.
		*/
		EDIT_BOX_INPUT_FLAG_SENSITIVE = 1,
		/**
		* This flag is a hint to the implementation that during text editing,
		* the initial letter of each word should be capitalized.
		*/
		EDIT_BOX_INPUT_FLAG_INITIAL_CAPS_WORD = 2,
		/**
		* This flag is a hint to the implementation that during text editing,
		* the initial letter of each sentence should be capitalized.
		*/
		EDIT_BOX_INPUT_FLAG_INITIAL_CAPS_SENTENCE = 3,
		/**
		* Capitalize all characters automatically.
		*/
		EDIT_BOX_INPUT_GLAG_INITIAL_CAPS_ALL_CHARACTERS = 4
    };

    /**
     * \brief Class for edit box.
     *
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
         * Set the font color of the widget's text.
         * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
         *              red, green and blue components respectively.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
         */
        virtual int setFontColor(const int color);

        /**
         * Set a text in the edit box that acts as a placeholder when an
         * edit box is empty.
         * @param text The given text.
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
         * @deprecated Use setInputMode and setInputFlag instead.
         * @param editBoxMode The given mode.
         */
        virtual void setEditMode(EditBoxMode editBoxMode);

        /**
         * Set the input mode of the edit box.
         * @param inputMode One of the EditBoxInputMode constants.
         */
        virtual void setInputMode(EditBoxInputMode inputMode);

        /**
         * Set the input flags that are to be applied to the edit box.
         * @param inputFlag One of the EditBoxInputFlag constants.
         */
        virtual void setInputFlag(EditBoxInputFlag inputFlag);

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

/*! @} */
