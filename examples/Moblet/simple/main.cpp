/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
				printf("Pressed left");
				break;
			case MAK_RIGHT:
				printf("Pressed right");
				break;
			case MAK_UP:
				printf("Pressed up");
				break;
			case MAK_DOWN:
				printf("Pressed down");
				break;
			case MAK_SOFTRIGHT:
			case MAK_0:
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
