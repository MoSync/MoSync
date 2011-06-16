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
 * @file Moblet.h
 * @author Emma Tresanszki
 *
 * This file contains the moblet that manages the application.
 */

#ifndef NATIVEUIMOBLET_H_
#define NATIVEUIMOBLET_H_

#include <maapi.h>

// Include the library for the event manager,
// which manages the application main loop.
#include <MAUtil/Moblet.h>

// Include library for string handling.
#include <MAUtil/String.h>

// The header for the main screen.
#include "PlayScreen.h"

namespace RockPaperScissors
{

/**
 * The Moblet that manages the application and handles events.
 */
class RockPaperScissorsMoblet : public MAUtil::Moblet
{
public:
	/**
	 * Constructor.
	 */
	RockPaperScissorsMoblet();

	/**
	 * Get the single instance of this Moblet.
	 * Returns a reference to the moblet instance,
	 * so that we can access it from outside its class.
	 * @return The Moblet instance.
	 */
	static RockPaperScissorsMoblet* getInstance();

	/**
	 * Method called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent();

private:
	/** The screen of the application. **/
	PlayScreen* mMainScreen;
};

} // namespace RockPaperScissors

#endif // NATIVEUIMOBLET_H_
