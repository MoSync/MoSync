/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file WebViewTwitter.cpp
 * @author Ali Sarrafi
 *
 * Application that reads Twitter flows.
 *
 * This program illustrates how to use the jQTouch JavAScript
 * library in a MoSync WebView application. The program also
 * shows how to communicate from JavaScript to C++.
 *
 * The entire UI and most of the application logic is implemented
 * in JavaScript.
 */

#include <josync/WebAppMoblet.h>	// Moblet for web applications.

using namespace MAUtil;
using namespace josync;

/**
 * The application class.
 */
class TwitterMoblet : public WebAppMoblet
{
public:
	TwitterMoblet()
	{
		enableWebViewMessages();
		getWebView()->disableZoom();
		showPage("index.html");
	}
};
// End of class TwitterMoblet

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new TwitterMoblet());
	return 0;
}
