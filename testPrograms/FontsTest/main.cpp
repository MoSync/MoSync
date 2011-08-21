/*
 * main.cpp
 *
 *  Created on: May 11, 2011
 *      Author: emma tresanszki
 */


// Include the library for the event manager, that manages the application main loop.
#include <MAUtil/Moblet.h>

// Include the header file for this application's moblet.
#include "Moblet.h"

using namespace MAUtil;

// The entry point for the application - the place where processing starts.
extern "C" int MAMain()
{
	// Create an instance for WikiMoblet & run it to start the application.
	MAUtil::Moblet::run(Test::TestMoblet::getInstance());

	//MyMoblet will run until it is closed by the user pressing key 0. When
	//it's closed we end our program in a well-behaved way by returning zero.
	return 0;
}
