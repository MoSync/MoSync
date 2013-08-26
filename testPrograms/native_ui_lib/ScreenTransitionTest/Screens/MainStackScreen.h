/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
	class VerticalLayout;
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

		virtual void createUI();

		virtual void addTitleLabel();

		virtual void addInfoLabel();

		virtual void addFooter();

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
		 * Screen's layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

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
