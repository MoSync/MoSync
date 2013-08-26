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
 * @author Emma Tresanszki
 *
 * This application provides an example on how to use the Native UI wrapper, and
 * specifically the video view widget.
 *
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
	// Create an instance for VideoMoblet & run it to start the application.
	MAUtil::Moblet::run(Test::VideoMoblet::getInstance());

	//MyMoblet will run until it is closed by the user pressing key 0. When
	//it's closed we end our program in a well-behaved way by returning zero.
	return 0;
}
