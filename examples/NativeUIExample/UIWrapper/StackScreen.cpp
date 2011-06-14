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

	void StackScreen::push(Screen* screen)
	{
		maWidgetStackScreenPush(getWidgetHandle(), screen->getWidgetHandle());
		mStack.add(screen);
	}

	void StackScreen::pop()
	{
		if (mStack.size() > 0)
		{
			maWidgetStackScreenPop(getWidgetHandle());
		}
	}

	/**
	 * This method is called when there is an event for this widget.
	 * It passes on the event to the widget's listener if one is set.
	 * Note: You can either use an event listener or override this
	 * method in a sublclass to handle events.
	 * @param widgetEventData The data for the widget event.
	 */
	void StackScreen::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		if (MAW_EVENT_STACK_SCREEN_POPPED == widgetEventData->eventType)
		{
			if (mStack.size() > 0)
			{
				//mStack[mStack.size() - 1]->hide();
				mStack.resize(mStack.size() - 1);
			}
		}
	}

	} // namespace UI
} // namespace MoSync
