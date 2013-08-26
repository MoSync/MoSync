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

/**
 * @file Controller.h
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief App main controller.
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../Observers/TransitionsScreenObserver.h"
#include "../Observers/LastScreenObserver.h"

namespace ScreenTransitionTest
{
	class LastScreen;
	class MainStackScreen;

	class Controller:
		public TransitionsScreenObserver,
		public LastScreenObserver
	{
	public:
		/**
		 * Constructor.
		 */
		Controller();

		/**
		 * Destructor.
		 */
		virtual ~Controller();

		/**
		 * Tell the observer to show the last screen.
		 */
		virtual void showLastScreen(int transType);

		/**
		 * Tell the observer to hide the last screen.
		 */
		virtual void hideLastScreen();

		/**
		 * Android runtime sends back key event before stack screen pop.
		 * This has been noted and soon the Android runtime will automatically
		 * pop the screen from stack when back button is pressed and send a pop
		 * event instead without a key event (only for non-root screens).
		 */
		virtual void handleAndroidBackButton();

	public:
		/**
		 * Stack screen to show.
		 */
		MainStackScreen* mStackScreen;

		/**
		 * Last screen to show.
		 */
		LastScreen* mSimpleScreen;

		/*
		 * True if last screen is shown, false otherwise.
		 */
		bool mIsLastScreenShown;
	};

} //end of ScreenTransitionTest


#endif /* CONTROLLER_H_ */
