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
 * This template application shows how to extend the
 * functionality in HTML5/JS with custom code in C++.
 */

#include <Wormhole/HybridMoblet.h>

#include "MAHeaders.h" // Defines BEEP_WAV

// Namespaces we want to access.
using namespace MAUtil;
using namespace NativeUI;
using namespace Wormhole;

/**
 * The application class.
 */
class MyMoblet : public HybridMoblet
{
public:
	MyMoblet()
	{
		// Show the start page.
		showPage("index.html");

		// Set the sound used by the PhoneGap beep notification API.
		// BEEP_WAV is defined in file Resources/Resources.lst.
		// Below we add our own beep message, to illustrate how to
		// invoke custom C++ code from JavaScript. Do not confuse these
		// two ways of playing a beep sound.
		setBeepSound(BEEP_WAV);

		// Register functions to handle custom messages sent
		// from JavaScript.
		addMessageFun(
			"Vibrate",
			(FunTable::MessageHandlerFun)&MyMoblet::vibrate);
		addMessageFun(
			"Beep",
			(FunTable::MessageHandlerFun)&MyMoblet::beep);
	}

	void vibrate(Wormhole::MessageStream& message)
	{
		int duration = MAUtil::stringToInteger(message.getNext());
		maVibrate(duration);
	}

	void beep(Wormhole::MessageStream& message)
	{
		// This is how to play a sound using MoSync API.
		maSoundPlay(BEEP_WAV, 0, maGetDataSize(BEEP_WAV));
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	(new MyMoblet())->enterEventLoop();
	return 0;
}
