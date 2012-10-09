/**
 * @file main.cpp
 *
 * This file contains the support code at the C++ level for an HTML5/JS
 * application that can access device services from JavaScript.
 *
 * You don't need to change anything in this code file unless you
 * wish to add support for functions not available out-of-the box
 * in wormhole.js.
 */

#include <Wormhole/HybridMoblet.h>

#include "MAHeaders.h" // Defines BEEP_WAV

// Namespaces we want to access.
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.

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
	}

	virtual ~MyMoblet()
	{
		// Add cleanup code as needed.
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
