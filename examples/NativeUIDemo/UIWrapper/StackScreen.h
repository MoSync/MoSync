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

		/**
		 * Get the number of screens in the stack screen.
		 * @return Number of screens.
		 */
		virtual int getStackSize();

		/**
		 * Push a screen onto the stack screen.
		 * This screen becomes the visible topmost screen.
		 * @param screen The screen to push and show.
		 */
		virtual void push(Screen* screen);

		/**
		 * Pop a screen from the stack screen.
		 * The previous screen becomes the visible topmost screen.
		 */
		virtual void pop();

		/**
		 * This method is called when there is an event for this widget.
		 *
		 * Note: If you subclass StackScreen and override handleWidgetEvent,
		 * make sure to call the method in this class to keep the stack
		 * size logic working. Or just use a widget event listener.
		 *
		 * @param widgetEventData The data for the widget event.
		 */
		virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

	private:

		/**
		 * List with the screens in the stack screen.
		 */
		MAUtil::Vector<Screen*> mStack;
	};

	} // namespace UI
} // namespace MoSync

#endif
