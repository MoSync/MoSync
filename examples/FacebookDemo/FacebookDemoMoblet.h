/* Copyright (C) 2011 MoSync AB

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

/*
 * FacebookDemoMoblet.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
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


/**
 * Moblet to be used as a template for a Native UI application.
 */
class FacebookDemoMoblet : public MAUtil::Moblet
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	FacebookDemoMoblet(const MAUtil::String &appId, const MAUtil::String &appSecret);

	void keyPressEvent(int keyCode, int nativeCode);
	void customEvent(const MAEvent &event);

	~FacebookDemoMoblet();
private:
	void initializeFacebook(const MAUtil::String &appId, const MAUtil::String &appSecret);
	void createGUI();

private:
	//publish
	void uploadProfilePhoto(FacebookDemoGUI::ListScreen *menu);
	void addLinkOnWall(FacebookDemoGUI::ListScreen *menu);
	void addPostOnWall(FacebookDemoGUI::ListScreen *menu);
	void addStatusMessageOnWall(FacebookDemoGUI::ListScreen *menu);
	//connections
	void addActivitiesButton(FacebookDemoGUI::ListScreen *menu);
	void addAlbumsButton(FacebookDemoGUI::ListScreen *menu);
	void addBooksButton(FacebookDemoGUI::ListScreen *menu);
	void addCheckinsButton(FacebookDemoGUI::ListScreen *menu);
	void addFeedButton(FacebookDemoGUI::ListScreen *menu);
	void addHomeButton(FacebookDemoGUI::ListScreen *menu);
	void addInterestsButton(FacebookDemoGUI::ListScreen *menu);
	void addLikesButton(FacebookDemoGUI::ListScreen *menu);
	void addLinksButton(FacebookDemoGUI::ListScreen *menu);
	void addMusicButton(FacebookDemoGUI::ListScreen *menu);
	void addPhotosButton(FacebookDemoGUI::ListScreen *menu);
	void addPictureButton(FacebookDemoGUI::ListScreen *menu);
	void addPostsButton(FacebookDemoGUI::ListScreen *menu);
	void addTelevisionButton(FacebookDemoGUI::ListScreen *menu);
	void addEventsButton(FacebookDemoGUI::ListScreen *menu);
	void addFriendsButton(FacebookDemoGUI::ListScreen *menu);
	void addFriendListsButton(FacebookDemoGUI::ListScreen *menu);
	void addNotesButton(FacebookDemoGUI::ListScreen *menu);
	void addStatusMessagesButton(FacebookDemoGUI::ListScreen *menu);


private:
	FacebookManager 						*mFacebookManager;
	FacebookDemoGUI::ListScreen				*mMainMenu;
	FacebookDemoGUI::FacebookLoginScreen 	*mLoginScreen;


	int 							*mPixels;
	int 							mPictureWidth;
	int 							mPictureHeight;

	MAUtil::String 					mPicture;
};

#endif /* FACEBOOKDEMOMOBLET_H_ */
