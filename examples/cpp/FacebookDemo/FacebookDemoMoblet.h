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

/*
 * FacebookDemoMoblet.h
 */

#ifndef FACEBOOKDEMOMOBLET_H_
#define FACEBOOKDEMOMOBLET_H_

#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <IX_WIDGET.h>

#include "Facebook/FacebookManager.h"
#include "GUI/ListScreen.h"
#include "GUI/FacebookLoginScreen.h"
#include "GUI/MainScreen.h"


/**
 * Class that creates the main screen and handles key press events
 */
class FacebookDemoMoblet : public MAUtil::Moblet, public NativeUI::WebViewListener
{
public:
	/**
	 * The constructor creates the user interface and the FacebookManager
	 */
	FacebookDemoMoblet(const MAUtil::String &appId);

	virtual void webViewHookInvoked( NativeUI::WebView* webView, int hookType,
			MAHandle urlData);

	/**
	 * WebViewListener overrides
	 */
	virtual void webViewContentLoading(
			NativeUI::WebView* webView,
			const int webViewState) {}

	/**
	 * destructor.
	 */
	~FacebookDemoMoblet();

	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent() GCCATTRIB(noreturn);
private:
	void login();

	/**
	 * creates the FacebookManager object, that handles the requests to Facebook, retrieving data and publishing
	 */
	void initializeFacebook(const MAUtil::String &appId);

	/**
	 * creates the GUI
	 */
	void createGUI();


	MAUtil::String extractAccessToken(const char *newurl);

private:
	/**
	 * creation of buttons for the main menu
	 */


	/**
	 * Creates a button and adds it to the main menu
	 * Adds on the button a command that sends the publish request to Facebook
	 */
	void addLinkOnWall(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addPostOnWall(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addStatusMessageOnWall(FacebookDemoGUI::FacebookDemoScreen *menu);

	/**
	 * Creates a button and adds it to the main menu
	 * Adds on the button a command that sends the connection request to Facebook
	 */
	void addActivitiesButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addAlbumsButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addBooksButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addCheckinsButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addFeedButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addHomeButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addInterestsButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addLikesButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addLinksButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addMusicButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addPhotosButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addPictureButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addPostsButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addTelevisionButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addEventsButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addFriendsButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addFriendListsButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addNotesButton(FacebookDemoGUI::FacebookDemoScreen *menu);
	void addStatusMessagesButton(FacebookDemoGUI::FacebookDemoScreen *menu);

private:
	/**
	 * FacebookManager object: handles the requests to Facebook, retrieving data and publishing
	 */
	FacebookManager							*mFacebookManager;
	FacebookDemoGUI::MainScreen				*mMainScreen;
	FacebookDemoGUI::FacebookLoginScreen 	*mLoginScreen;
};

#endif /* FACEBOOKDEMOMOBLET_H_ */
