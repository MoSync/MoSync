/*
Copyright (C) 2013 MoSync AB

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

#ifndef MAINSTACKSCREEN_H_
#define MAINSTACKSCREEN_H_

#include <NativeUI/StackScreen.h>
#include <NativeUI/StackScreenListener.h>
#include <NativeUI/ButtonListener.h>

#include "../Observers/TransitionsScreenObserver.h"

namespace NativeUI
{
	class Button;
	class Screen;
}

namespace ScreenTransitionTest
{
	class TransitionsScreen;

	class MainStackScreen:
		public NativeUI::StackScreen,
		private NativeUI::ButtonListener,
		private NativeUI::StackScreenListener
	{

	public:
		/**
		 * Constructor.
		 */
			MainStackScreen(TransitionsScreenObserver& observer);

		/**
		 * Destructor.
		 */
		~MainStackScreen();

	private:

		virtual void addMainScreen();

		/**
		 * This method is called when there is an touch-down event for
		 * a button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonPressed(NativeUI::Widget* button) {};

		/**
		 * This method is called when there is an touch-up event for
		 * a button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonReleased(NativeUI::Widget* button) {};

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(NativeUI::Widget* button);

		/**
		 * This method is called when a screen has been popped from a stack
		 * screen.
		 * @param stackScreen The stack screen object that generated the event.
		 * @param fromScreen The screen that was popped from the stack screen.
		 * @param toScreen The screen that will be shown.
		 */
		virtual void stackScreenScreenPopped(
			NativeUI::StackScreen* stackScreen,
			NativeUI::Screen* fromScreen,
			NativeUI::Screen* toScreen);

	private:
		/**
		 * Main screen to push.
		 */
		NativeUI::Screen* mMainScreen;

		/**
		 * Transitions screen to show.
		 */
		TransitionsScreen* mTransitionsScreen;

		/**
		 * Used to push transitions screen.
		 */
		NativeUI::Button* mPushScreen_Button;
	};
} //ScreenTransitionTest

#endif /* MAINSTACKSCREEN_H_ */
