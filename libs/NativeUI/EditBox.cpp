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
 * @file EditBox.cpp
 * @author Bogdan Iusco
 *
 * Class for edit box.
 * You can use this widget to gather small amounts of text from the user.
 * The edit box supports the use of an observer to handle editing-related
 * events.
 */

#include "EditBox.h"
#include "EditBoxListener.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 * @param category The type of the edit box.
	 */
	EditBox::EditBox(EditBoxCategory category): Widget(MAW_EDIT_BOX)
	{
		if (category == EDIT_BOX_MULTI_LINE)
		{
			// Set multi line on iOS platform.
			this->setPropertyInt(MAW_EDIT_BOX_MODE, MAW_EDIT_BOX_MODE_MULTI_LINE);
			// Set multi line on Android and Windows Phone 7 platforms.
			this->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
		}
	}

	/**
	 * Destructor.
	 */
	EditBox::~EditBox()
	{
		mEditBoxListeners.clear();
	}

	/**
	 * Set the text entered in the edit box.
	 * @param text The given text.
	 */
	void EditBox::setText(const MAUtil::String& text)
	{
		this->setProperty(MAW_EDIT_BOX_TEXT, text);
	}

	/**
	 * Get the text entered in the edit box.
	 * @return The text entered in the edit box.
	 */
	MAUtil::String EditBox::getText()
	{
		return this->getPropertyString(MAW_EDIT_BOX_TEXT);
	}

	/**
	 * Set the font color of the widget's text.
	 * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
	 *			  red, green and blue components respectively.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
	 */
	int EditBox::setFontColor(const int color)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "0x%.6X", color);
		return this->setProperty(MAW_EDIT_BOX_FONT_COLOR, buffer);
	}

	/**
	 * Set the font color of the placeholder text when the edit box is empty.
	 * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
	 *			  red, green and blue components respectively.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
	 */
	int EditBox::setPlaceholderFontColor(const int color)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "0x%.6X", color);
		return this->setProperty(MAW_EDIT_BOX_PLACEHOLDER_FONT_COLOR, buffer);
	}

	/**
	 * Set a text in the edit box that acts as a placeholder when an
	 * edit box is empty.
	 * @param text The given text.
	 */
	void EditBox::setPlaceholder(const MAUtil::String& text)
	{
		this->setProperty(MAW_EDIT_BOX_PLACEHOLDER, text);
	}

	/**
	 * Show the virtual keyboard.
	 * The focus will be set to this widget.
	 */
	void EditBox::showKeyboard()
	{
		this->setProperty(MAW_EDIT_BOX_SHOW_KEYBOARD, "true");
	}

	/**
	 * Hide the virtual keyboard.
	 */
	void EditBox::hideKeyboard()
	{
		this->setProperty(MAW_EDIT_BOX_SHOW_KEYBOARD, "false");
	}

	/**
	 * Set the mode of the edit box.
	 * @deprecated Use setInputMode and setInputFlag instead.
	 * @param editBoxMode The given mode.
	 */
	void EditBox::setEditMode(EditBoxMode editBoxMode)
	{
		MAUtil::String mode;
		switch (editBoxMode)
		{
			case EDIT_BOX_MODE_TEXT:
				mode = "text";
				break;
			case EDIT_BOX_MODE_PASSWORD:
				mode = "password";
				break;
		}

		this->setProperty(MAW_EDIT_BOX_EDIT_MODE, mode);
	}

	/**
	 * Set the input mode of the edit box.
	 * @param inputMode One of the EditBoxInputMode constants.
	 */
	void EditBox::setInputMode(EditBoxInputMode inputMode)
	{
		this->setPropertyInt(MAW_EDIT_BOX_INPUT_MODE, inputMode);
	}

	/**
	 * Set the input flags that are to be applied to the edit box.
	 * @param inputFlag One of the EditBoxInputFlag constants.
	 */
	void EditBox::setInputFlag(EditBoxInputFlag inputFlag)
	{
		this->setPropertyInt(MAW_EDIT_BOX_INPUT_FLAG, inputFlag);
	}

	/**
	 * Set the number of lines.
	 * Makes the edit box exactly this many lines tall. Note that setting this value
	 * overrides any other (minimum / maximum) number of lines or height setting.
	 * A single line edit box will set this value to 1.
	 * Available on Android only.
	 *
	 * @param lines The number of lines.
	 */
	void EditBox::setLinesNumber(int lines)
	{
		this->setPropertyInt(MAW_EDIT_BOX_LINES_NUMBER, lines);
	}

	/**
	 * Makes the edit box at most this many lines tall.
	 * Setting this value overrides any other (maximum) height setting.
	 * Available on Android only.
	 *
	 * @param maxLines The maximum number of lines.
	 */
	void EditBox::setMaxLines(int maxLines)
	{
		this->setPropertyInt(MAW_EDIT_BOX_MAX_LINES, maxLines);
	}

	/**
	 * Makes the edit box at least this many lines tall.
	 * Setting this value overrides any other (minimum) height setting.
	 * Available on Android only.
	 *
	 * @param minLines The minimum number of lines.
	 */
	void EditBox::setMinLines(int minLines)
	{
		this->setPropertyInt(MAW_EDIT_BOX_MIN_LINES, minLines);
	}

	/**
	 * Sets the maximum input length of the edit box.
	 * Setting this value enables multiline input mode by default.
	 * Available on Android, iOS and Windows Phone.
	 *
	 * @param maxLength The maximum length.
	 */
	void EditBox::setMaxLength(int maxLength)
	{
		this->setPropertyInt(MAW_EDIT_BOX_MAX_LENGTH, maxLength);
	}

	/**
	 * Gets the maximum input length of the edit box.
	 * Available on Android, iOS and WindowsPhone.
	 *
	 * @return Maximum input length.
	 */
	int EditBox::getMaxLength(int maxLength)
	{
		return this->getPropertyInt(MAW_EDIT_BOX_MAX_LENGTH);
	}

	/**
	* Set the autocorrection type.
	* Enable/disable the auto correction during typing. With auto correction
	* enabled, the text object tracks unknown words and suggests a more
	* suitable replacement candidate to the user, replacing the typed text
	* automatically unless the user explicitly overrides the action.
	* The default value is #MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEAFULT.
	* Platform: iOS.
	* @param One of the following value:
	*  MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT,
	*  MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE,
	*  MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE.
	*/
	void EditBox::setAutocorrectionType(
		EditBoxAutocorrectionType autocorrectionType)
	{
		int autocorrectionTypeInt;
		switch (autocorrectionType)
		{
		case EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE:
			autocorrectionTypeInt = MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE;
			break;
		case EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE:
			autocorrectionTypeInt = MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE;
			break;
		case EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT:
		default:
			autocorrectionTypeInt = MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT;
			break;
		}
		this->setPropertyInt(MAW_EDIT_BOX_AUTOCORRECTION_TYPE,
			autocorrectionTypeInt);
	}

	/**
	 * Get the autocorrection type.
	 * Platform: iOS.
	 * @return One of the following value:
	 *  MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT,
	 *  MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE,
	 *  MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE.
	 */
	EditBoxAutocorrectionType EditBox::getAutocorrectionType()
	{
		int autocorrectionTypeInt = this->getPropertyInt(
			MAW_EDIT_BOX_AUTOCORRECTION_TYPE);

		EditBoxAutocorrectionType autocorrectionType;
		switch (autocorrectionTypeInt)
		{
		case MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE:
			autocorrectionType = EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE;
			break;
		case MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE:
			autocorrectionType = EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE;
			break;
		case MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT:
		default:
			autocorrectionType = EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT;
			break;
		}
		return autocorrectionType;
	}

	/**
	 * Set the horizontal alignment of the text inside the edit box.
	 * The default value is MAW_ALIGNMENT_LEFT;
	 * @param alignment One of the constants:
	 * - MAW_ALIGNMENT_LEFT
	 * - MAW_ALIGNMENT_CENTER
	 * - MAW_ALIGNMENT_RIGHT
	 */
	void EditBox::setTextHorizontalAlignment(const MAUtil::String& alignment)
	{
		this->setProperty(MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT, alignment);
	}

	/**
	 * Get the horizontal alignment of the text inside the edit box.
	 * @return One of the constants:
	 * - MAW_ALIGNMENT_LEFT
	 * - MAW_ALIGNMENT_CENTER
	 * - MAW_ALIGNMENT_RIGHT
	 */
	MAUtil::String EditBox::getTextHorizontalAlignment()
	{
		return this->getPropertyString(MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT);
	}

	/**
	 * Add an edit box event listener.
	 * @param listener The listener that will receive edit box events.
	 */
	void EditBox::addEditBoxListener(EditBoxListener* listener)
	{
		addListenerToVector(mEditBoxListeners, listener);
	}

	/**
	 * Remove the edit box listener.
	 * @param listener The listener that receives edit box events.
	 */
	void EditBox::removeEditBoxListener(EditBoxListener* listener)
	{
		removeListenerFromVector(mEditBoxListeners, listener);
	}

	/**
	 * This method is called when there is an event for this widget.
	 * It passes on the event to all widget's listeners.
	 * @param widgetEventData The data for the widget event.
	 */
	void EditBox::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		Widget::handleWidgetEvent(widgetEventData);

		if (MAW_EVENT_EDIT_BOX_EDITING_DID_BEGIN == widgetEventData->eventType)
		{
			for (int i = 0; i < mEditBoxListeners.size(); i++)
			{
				mEditBoxListeners[i]->editBoxEditingDidBegin(this);
			}
		}
		else if (MAW_EVENT_EDIT_BOX_EDITING_DID_END == widgetEventData->eventType)
		{
			for (int i = 0; i < mEditBoxListeners.size(); i++)
			{
				mEditBoxListeners[i]->editBoxEditingDidEnd(this);
			}
		}
		else if (MAW_EVENT_EDIT_BOX_TEXT_CHANGED == widgetEventData->eventType)
		{
			MAUtil::String text = this->getText();
			for (int i = 0; i < mEditBoxListeners.size(); i++)
			{
				mEditBoxListeners[i]->editBoxTextChanged(this, text);
			}
		}
		else if (MAW_EVENT_EDIT_BOX_RETURN == widgetEventData->eventType)
		{
			for (int i = 0; i < mEditBoxListeners.size(); i++)
			{
				mEditBoxListeners[i]->editBoxReturn(this);
			}
		}
	}

} // namespace NativeUI
