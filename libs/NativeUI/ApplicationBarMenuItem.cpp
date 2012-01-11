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
 * @file ApplicationBarMenuItem.cpp
 * @author Ciprian Filipas
 *
 * Class that represents an applicationBarMenuItem. Note that
 * this widget must only be used as a child for an application
 * bar on Windows Phone 7
 */

#include "ApplicationBarMenuItem.h"
#include "ButtonListener.h"

namespace NativeUI
{
	/**
	* Constructor.
	*/
	ApplicationBarMenuItem::ApplicationBarMenuItem() :
			Widget(MAW_APPLICATION_BAR_MENU_ITEM)
	{
		this->setText("Click Me!");
	}

	/**
	 * Destructor.
	 */
	ApplicationBarMenuItem::~ApplicationBarMenuItem()
	{
	}

	/**
	 * Set the text displayed by the widget.
	 * @param text The given text.
	 */
	void ApplicationBarMenuItem::setText(const MAUtil::String& text)
	{
		this->setProperty(MAW_APPLICATION_BAR_BUTTON_TEXT, text.c_str());
	}

	/**
	 * Get the text displayed by the widget.
	 * @param text The given text.
	 */
	MAUtil::String ApplicationBarMenuItem::getText()
	{
		return this->getPropertyString(MAW_BUTTON_TEXT);
	}

	/**
	 * Add an button event listener.
	 * @param listener The listener that will receive button events.
	 */
	void ApplicationBarMenuItem::addButtonListener(ButtonListener* listener)
	{
		addListenerToVector(mButtonListeners, listener);
	}

	/**
	 * Remove the button listener.
	 * @param listener The listener that receives button events.
	 */
	void ApplicationBarMenuItem::removeButtonListener(ButtonListener* listener)
	{
		removeListenerFromVector(mButtonListeners, listener);
	}

	/**
	 * This method is called when there is an event for this widget.
	 * It passes on the event to all widget's listeners.
	 * @param widgetEventData The data for the widget event.
	 */
	void ApplicationBarMenuItem::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		Widget::handleWidgetEvent(widgetEventData);

		if (MAW_EVENT_POINTER_PRESSED == widgetEventData->eventType)
		{
			for (int i = 0; i < mButtonListeners.size(); i++)
			{
				mButtonListeners[i]->buttonPressed(this);
			}
		}
		else if (MAW_EVENT_POINTER_RELEASED == widgetEventData->eventType)
		{
			for (int i = 0; i < mButtonListeners.size(); i++)
			{
				mButtonListeners[i]->buttonReleased(this);
			}
		}
		else if (MAW_EVENT_CLICKED == widgetEventData->eventType)
		{
			for (int i = 0; i < mButtonListeners.size(); i++)
			{
				mButtonListeners[i]->buttonClicked(this);
			}
		}
	}
} // namespace NativeUI
