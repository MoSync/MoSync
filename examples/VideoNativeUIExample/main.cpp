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

/** @file main.cpp
 *
 * This application provides an example on how to use the Native UI wrapper, and
 * specifically the video view widget.
 *
 * @author Emma Tresanszki
 */

// Include the library for the event manager, that manages the application
// main loop.
#include <MAUtil/Moblet.h>

// Include the header file for this application's moblet.
#include "Moblet.h"

using namespace MAUtil;

/**
 * The entry point for the application - the place where processing starts.
 */
extern "C" int MAMain()
{
	// Create an instance for WikiMoblet & run it to start the application.
	MAUtil::Moblet::run(Test::TestMoblet::getInstance());

	//MyMoblet will run until it is closed by the user pressing key 0. When
	//it's closed we end our program in a well-behaved way by returning zero.
	return 0;
}
