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

/** \file main.cpp
* This applications demonstrates basic usage of the Moblet class.
* A Moblet is a class that wraps the standard procedural MoSync
* event system in a more OOP-style interface. Instead of implementing
* an event loop to recieve events, you implement a virtual function for
* each type of MoSync event. These include key events and close events.
*
* The static function Moblet::run() implements the actual event loop. In
* addition to providing a more object-oriented approach, using Moblets
* makes it easier to implement well-behaved MoSync programs. For instance,
* it will respond to close events by default.
*/

#include <conprint.h>
#include <MAUtil/Moblet.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:

	/**
	 * Your Moblet's constructor is a good place to do initialization and
	 * setting up the member variables in your class. In this case, we're
	 * initializing a counter that keeps track of how many times the user
	 * has released a key. We also print a message, in order to illustrate
	 * when the constructor gets called.
	**/

	MyMoblet() : numKeyReleases(0) {
		printf("Hello moblet!\n");
	}

	/**
	 * The Moblet class inherits KeyListener, and automatically registers itself
	 * to recieve key events. Therefore, it must implement the functions keyPressEvent
	 * and KeyReleaseEvent. There are default implementations of these in the Moblet
	 * class, but it's likely you'll want to reimplement if your moblet is to be
	 * interactive. In this example, we will simply react to some key being pressed
	 * by printing out which key it was.
	**/

	void keyPressEvent(int keyCode, int nativeCode) {
		switch(keyCode) {
			case MAK_LEFT:
				printf("Pressed left (%i)", nativeCode);
				break;
			case MAK_RIGHT:
				printf("Pressed right (%i)", nativeCode);
				break;
			case MAK_UP:
				printf("Pressed up (%i)", nativeCode);
				break;
			case MAK_DOWN:
				printf("Pressed down (%i)", nativeCode);
				break;
			case MAK_SOFTRIGHT:
			case MAK_0:
			case MAK_BACK:
				maExit(0);
			default:
				printf("Presscode %i/%i\n", keyCode, nativeCode);
				break;
		}
	}

	/**
	 * In the keyRelaseEvent, we use the numKeyReleases variable
	 * to keep track of how many times a key has been released.
	 * Adimttedly contrived, it still illustrates the basic idea
	 * of managing state in Moblets using member variables.
    **/

	void keyReleaseEvent(int keyCode, int nativeCode) {
		printf("%d keys have been released\n", ++numKeyReleases);
	}

private:
	int numKeyReleases;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new MyMoblet());
	return 0;
}
