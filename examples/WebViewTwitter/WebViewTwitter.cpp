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

#include <ma.h>						// MoSync API (base API).
#include <maheap.h>					// C memory allocation functions.
#include <mastring.h>				// C String functions.
#include <mavsprintf.h>				// sprintf etc.
#include <NativeUI/WebAppMoblet.h>	// Moblet for web applications.
#include <conprint.h>				// Debug logging (you must build in
									// debug mode for output to show).
using namespace MAUtil;
using namespace NativeUI;

/**
 * The application class.
 */
class TwitterMoblet : public WebAppMoblet
{
public:
	TwitterMoblet()
	{
		getWebView()->disableZoom();
		showPage("index.html");
	}

	/**
	 * Here we handle messages sent from JavaScript.
	 */
	void handleWebViewMessage(WebView* webView, WebViewMessage& message)
	{
		if (message.is("AddToFavorites"))
		{
			// Save the user to favourites list.
			addUserToFavourites(message.getParam("userName"));
		}
		else if (message.is("PageLoaded"))
		{
			// When the page is loaded, we read the favourites
			// list and display it in the WebView.
			loadAndDisplayFavorites();
		}
	}

	void addUserToFavourites(const MAUtil::String& userName)
	{
		// Add new Twitter user to list of favourites.
		MAUtil::String userList = loadFavourites();
		if (userList == "")
		{
			userList += userName;
		}
		else if (userList.find(userName) < 0)
		{
			userList += ",";
			userList += userName;
		}
		else
		{
			callJS("alert('User already in the list')");
			return;
		}

		// Save list on device.
		getFileUtil()->writeTextToFile(usersPath(), userList);

		// Display list in WebView.
		displayFavourites(userList);

		callJS("alert('User added to favorites')");
	}

	void loadAndDisplayFavorites()
	{
		MAUtil::String userList = loadFavourites();
		displayFavourites(userList);
	}

	MAUtil::String loadFavourites()
	{
		MAUtil::String userList;
		bool success = getFileUtil()->readTextFromFile(
			usersPath(),
			userList);
		if (success)
		{
			return userList;
		}
		else
		{
			return "";
		}
	}

	MAUtil::String usersPath()
	{
		return getFileUtil()->getLocalPath() + "SavedUsers";
	}

	/**
	 * Call JavaScript to display list of favourite twitter accounts.
	 */
	void displayFavourites(const MAUtil::String& userList)
	{
		MAUtil::String script = "setFavorites('";
		script += userList + "')";
		callJS(script);
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
