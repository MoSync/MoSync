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
 * @file StackScreen.cpp
 * @author Niklas Nummelin & Mikael Kindborg
 */

#include "StackScreen.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor.
	 */
	StackScreen::StackScreen() :
		Screen(MAW_STACK_SCREEN)
	{
	}

	/**
	 * Destructor.
	 */
	StackScreen::~StackScreen()
	{
	}

	/**
	 * Get the number of screens in the stack screen.
	 * @return Number of screens.
	 */
	int StackScreen::getStackSize()
	{
		return mStack.size();
	}

	/**
	 * Push a screen onto the stack screen.
	 * This screen becomes the visible topmost screen.
	 * @param screen The screen to push and show.
	 */
	void StackScreen::push(Screen* screen)
	{
		maWidgetStackScreenPush(getWidgetHandle(), screen->getWidgetHandle());
		mStack.add(screen);
	}

	/**
	 * Pop a screen from the stack screen.
	 * The previous screen becomes the visible topmost screen.
	 */
	void StackScreen::pop()
	{
		if (mStack.size() > 0)
		{
			maWidgetStackScreenPop(getWidgetHandle());
		}
	}

	/**
	 * This method is called when there is an event for this widget.
	 *
	 * Note: If you subclass StackScreen and override handleWidgetEvent,
	 * make sure to call the method in this class to keep the stack
	 * size logic working. Or just use a widget event listener.
	 *
	 * @param widgetEventData The data for the widget event.
	 */
	void StackScreen::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		if (MAW_EVENT_STACK_SCREEN_POPPED == widgetEventData->eventType)
		{
			if (mStack.size() > 0)
			{
				mStack.resize(mStack.size() - 1);
			}
		}
		else
		{
			Screen::handleWidgetEvent(widgetEventData);
		}
	}

	} // namespace UI
} // namespace MoSync
