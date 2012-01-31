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
 * @file WikiMoblet.h
 * @author Emma Tresanszki
 *
 * This file contains the moblet that manages the application.
 */

#ifndef WIKIMOBLET_H_
#define WIKIMOBLET_H_

// Include the syscalls library.
#include <maapi.h>

// Include the library for the event manager,
// which manages the application main loop.
#include <MAUtil/Moblet.h>

// Include the utility functions.
#include "Util.h"

// The default screen, that is displayed when application starts
#include "HomeScreen.h"

namespace WikiNativeUI
{

/**
 * The Moblet that will manage the application and handle events
 */
class WikiMoblet : public MAUtil::Moblet
{
	/*
	 * ctor
	 */
	WikiMoblet();

public:
	/**
	 * Get the single instance of this Moblet.
	 * @return The Moblet instance.
	 */
	static WikiMoblet *getInstance();

	/**
	 * Method called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent() GCCATTRIB(noreturn);

private:
	static WikiMoblet *mInstance;

	/** The main screen of the application. **/
	HomeScreen* mMainScreen;
};

}// namespace WikiNativeUI

#endif /* WIKIMOBLET_H_ */
