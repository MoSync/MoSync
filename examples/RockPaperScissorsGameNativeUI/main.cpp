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
 * @file main.cpp
 * @author Emma Tresanszki
 *
 * This application provides a very basic example of how to use some native UI widgets.
 * It is based on the game Rock-Paper-Scissors.
 * The application makes use of MoSync's Moblet framework to handle events.
 *
 * In Moblet.cpp we manage the entire application.
 * In Playscreen.cpp we define the UI elements.
 * And in Game.cpp we handle the game logic.
 
 * Note also that this example will not work on the MoSync emulator (MoRE)
 * because it has no Native UI support yet.
 */

// Include the class declaration for the application's Moblet.
#include "Moblet.h"

/**
 * This is the entry point for the application - the place where
 * processing starts.
 */
extern "C" int MAMain()
{
	// Create an instance for RockPaperScissorsMoblet & run it to start the
	// application.
	MAUtil::Moblet::run(new RockPaperScissors::RockPaperScissorsMoblet());

	// MyMoblet will run until it is closed by the user. When
	// it's closed we end our program in a well-behaved way by
	// returning zero.
	return 0;
}
