/**
 * @file main.cpp
 *
 * Sample web application packaged as a MoSync application.
 */

// Include Moblet for web applications.
#include <josync/WebAppMoblet.h>

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace josync; // Class WebAppMoblet

/**
 * The application class.
 */
class MyMoblet : public WebAppMoblet
{
public:
	MyMoblet()
	{
		// Remove this line to enable the user to
		// zoom the web page. To disable zoom is one
		// way of making web pages display in a
		// reasonable degault size on devices with
		// different screen sizes.
		getWebView()->disableZoom();

		// The page in the "LocalFiles" folder to
		// show when the application starts.
		showPage("index.html");
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
