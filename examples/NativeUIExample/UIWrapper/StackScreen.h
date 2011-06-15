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
 * @file StackScreen.h
 * @author Niklas Nummelin & Mikael Kindborg
 */

#ifndef MOSYNC_UI_STACKSCREEN_H_
#define MOSYNC_UI_STACKSCREEN_H_

#include <ma.h>
#include <MAUtil/Vector.h>
#include <IX_WIDGET.h>
#include "Screen.h"

namespace MoSync
{
	namespace UI
	{

	class StackScreen : public Screen
	{
	public:
		/**
		 * Constructor.
		 */
		StackScreen();

		/**
		 * Destructor.
		 */
		virtual ~StackScreen();

		virtual int getStackSize();

		virtual void push(Screen* screen);

		virtual void pop();

		/**
		 * This method is called when there is an event for this widget.
		 * It passes on the event to the widget's listener if one is set.
		 * Note: You can either use an event listener or override this
		 * method in a sublclass to handle events.
		 * @param widgetEventData The data for the widget event.
		 */
		virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

	private:
		MAUtil::Vector<Screen*> mStack;
	};

	} // namespace UI
} // namespace MoSync

#endif
