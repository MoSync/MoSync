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

#include "../Observers/FirstScreenObserver.h"
#include "../Observers/SecondScreenObserver.h"

namespace ScreenTransitionTest
{
	class FirstScreen;
	class SecondScreen;

	class Controller:
		public FirstScreenObserver,
		public SecondScreenObserver
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
		 * Tell the observer to show the second screen.
		 */
		virtual void showSecondScreen(int transType, const char* text);

		/**
		 * Tell the observer to hide the second screen.
		 */
		virtual void hideSecondScreen();

	public:
		/**
		 * First screen to show.
		 */
		FirstScreen* mFirstScreen;

		/**
		 * Second screen to show.
		 */
		SecondScreen* mSecondScreen;
	};

} //end of ScreenTransitionTest


#endif /* CONTROLLER_H_ */
