/**
 * @file main.cpp
 *
 * This file contains the support code at the C++ level for
 * an HTML5/JS application that has access to device services.
 *
 * You don't need to change anything in this code file unless
 * you wish to add support for functions that are not available
 * out-of-the box in wormhole.js.
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
		setBeepSound(BEEP_WAV);
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
