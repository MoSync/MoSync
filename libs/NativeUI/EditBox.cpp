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
     */
    EditBox::EditBox(): Widget(MAW_EDIT_BOX)
    {
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
    MAUtil::String EditBox::getText() const
    {
        return this->getPropertyString(MAW_EDIT_BOX_TEXT);
    }

    /**
     * Set a text in the edit box that acts as a placeholder when an
     * edit box is empty.
     * @param The given text.
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
