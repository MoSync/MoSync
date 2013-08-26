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

/** @file helloworld.cpp
*
* This application provides a very basic example of how to use MoSync
* to print the text "Hello World" to a device's screen. The code is very
* well commented so that you can see exactly what's happening at each step.
* The application makes use of MoSync's Moblet framework to handle events.
* Full documentation for this application can be found at the MoSync web site:
* http://www.mosync.com/documentation/manualpages/hello-world-deconstructed
*
* @author Chris Hughes
*/

//Include the header files for MoSync Moblets so that we can
//access the Moblet library code from our application.
#include <MAUtil/Moblet.h>

//Declare the MAUtil namespace so that we can use the short forms of
//identifiers in our code. This allows us to write, for example,  "Moblet"
//instead of "MAUtil::Moblet".
using namespace MAUtil;

//Create the wrapper for the entire application. It is here that we will manage
//the application and handle events. To create the wrapper we make our own
//implementation of the MoSync Moblet base class. There can be only one Moblet
//in an application.
class MyMoblet : public Moblet {

//Define our new class's public methods.
public:

	//First, the constructor, which we will call whenever we need an
	//instance of MyMoblet. In this case, the constructor consists
	//of three syscalls.
	MyMoblet() {
		//The first syscall sets the background colour.
		maSetColor(0xFFFFFF),
		//The second syscall writes the text "Hello World" to the backbuffer.
		maDrawText(0, 32, "Hello World!");
		//The third syscall copies the contents of the backbuffer to the
		//physical screen.
		maUpdateScreen();
	}

	//Next, a method for detecting key presses. This is a method we have
	//inherited from the Moblet base class, and here we will override that
	//method with some processing of our own.
	void keyPressEvent(int keyCode, int nativeCode) {
		//Close the application if the zero, back, or soft-right key is pressed.
		if(keyCode == MAK_0 || keyCode == MAK_BACK || keyCode == MAK_SOFTRIGHT)
		{
			close();
		}
	}

	//Finally, a code stub we might need to use later for another event type.
	void keyReleaseEvent(int keyCode, int nativeCode) {
		//to do: handle key releases - put your code here.
	}
};

//Now we get to the entry point for the application - the place where
//processing starts.
extern "C" int MAMain() {

	//Create the instance of MyMoblet
	MyMoblet myMoblet;

	// Run the Moblet to start the application.
	Moblet::run( &myMoblet );

	//MyMoblet will run until it is closed by the user pressing key 0. When
	//it's closed we end our program in a well-behaved way by returning zero.
	return 0;
};
