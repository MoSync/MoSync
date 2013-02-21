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
