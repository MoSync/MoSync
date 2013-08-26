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
 * @file main.cpp
 *
 * This application provides a very basic example of how to use some native UI widgets.
 * It is based on the game Rock-Paper-Scissors.
 * The application makes use of MoSync's Moblet framework to handle events.
 *
 * In Moblet.cpp we manage the entire application.
 * In Playscreen.cpp we define the UI elements.
 * And in Game.cpp we handle the game logic.
 *
 * Note also that this example will not work on the MoSync emulator (MoRE)
 * because it has no Native UI support yet.
 *
 * @author Emma Tresanszki
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
