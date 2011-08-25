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
 * FacebookMobletDemo.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#include <MAUtil/Set.h>
#include <mastring.h>

#include "FacebookDemoMoblet.h"
#include "Facebook/GetPermissions.h"
#include "Facebook/GraphAPI/GetConnections/Connections.h"

#include "GUI/ListItem.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Link.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Post.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/StatusMessage.h"

#include "Application/OpenMenuCommand.h"
#include "Application/ConnectionRequestCommand.h"
#include "Application/PublishRequestCommand.h"

#include "GUI/NativeUI/Screen.h"
#include "GUI/NativeUI/Image.h"

FacebookDemoMoblet::FacebookDemoMoblet(const MAUtil::String &appId, const MAUtil::String &appSecret)
{
	initializeFacebook(appId, appSecret);
	createGUI();

	MAUtil::String oAuthUrl = mFacebookManager->getOAuthUrl();
	LOG("\t\tOAuthUrl = %s", oAuthUrl.c_str());
	mLoginScreen->setUrl(oAuthUrl);
	mLoginScreen->show();
}

/**
 * Called when a key is pressed.
 */
void FacebookDemoMoblet::keyPressEvent(int keyCode, int nativeCode)
{
	LOG("\t\t!FacebookDemoMoblet::keyPressEvent keyCode=%d; ", keyCode);
	if (MAK_HOME == keyCode )
	{
		close();
	}
}

void FacebookDemoMoblet::customEvent(const MAEvent &event)
{
	if ( event.type != EVENT_TYPE_WIDGET )
	{
		return;
	}

	int eventType = ((MAWidgetEventData*) event.data)->eventType;
	if ( eventType == MAW_EVENT_WEB_VIEW_URL_CHANGED )
	{
		const int BUFFER_SIZE = 16384;
		char *newurl = new char[BUFFER_SIZE];
		strcpy((char*) newurl, mLoginScreen->getRedirectUrl().c_str());
		LOG("\n\t\t\tredirect url: %s", newurl);
		if (strstr(newurl, "fbconnect://") == newurl)
		{
			char *accessToken = strstr(newurl, "access_token=") + strlen("access_token=");
			char *expiresIn = strstr(newurl, "&expires_in=");
			accessToken[expiresIn - accessToken] = 0;

			mFacebookManager->setAccessToken(accessToken);

			LOG("\t\t\tacces token receivied: %s\n", accessToken);

			mMainMenu->show();
			mMainMenu->receiveKeyEvents(true);
		}
		delete []newurl;
	}
}


FacebookDemoMoblet::~FacebookDemoMoblet()
{
	delete mLoginScreen;
	delete mMainMenu;
}

void FacebookDemoMoblet::initializeFacebook(const MAUtil::String &appId, const MAUtil::String &appSecret)
{
	MAUtil::Set<MAUtil::String> permissions;
	//access_token
	permissions.insert("offline_access");

	GetPermissionsFor<User>::Retrieve::onlyConnections(permissions);
	GetPermissionsFor<User>::publishingData(permissions);
	GetPermissionsFor<Album>::allPosibleRequests(permissions);
	GetPermissionsFor<Checkin>::allPosibleRequests(permissions);
	GetPermissionsFor<Comment>::allPosibleRequests(permissions);
	GetPermissionsFor<Event>::allPosibleRequests(permissions);
	GetPermissionsFor<FriendList>::allPosibleRequests(permissions);
	GetPermissionsFor<Link>::allPosibleRequests(permissions);
	GetPermissionsFor<Note>::allPosibleRequests(permissions);
	GetPermissionsFor<Photo>::allPosibleRequests(permissions);
	GetPermissionsFor<Post>::allPosibleRequests(permissions);
	GetPermissionsFor<StatusMessage>::allPosibleRequests(permissions);

	mFacebookManager = new FacebookManager(appId, permissions);
}

void FacebookDemoMoblet::createGUI()
{
	using namespace FacebookDemoGUI;

	mLoginScreen = new FacebookLoginScreen();
	mMainMenu = new ListScreen();

	//publish
	uploadProfilePhoto(mMainMenu);
	addLinkOnWall(mMainMenu);
	addPostOnWall(mMainMenu);
	addStatusMessageOnWall(mMainMenu);

	//connections
	addActivitiesButton(mMainMenu);
	addAlbumsButton(mMainMenu);
	addBooksButton(mMainMenu);
	addCheckinsButton(mMainMenu);
	addFeedButton(mMainMenu);
	addHomeButton(mMainMenu);
	addInterestsButton(mMainMenu);
	addLikesButton(mMainMenu);
	addLinksButton(mMainMenu);
	addMusicButton(mMainMenu);
	addPhotosButton(mMainMenu);
	addPictureButton(mMainMenu);
	addPostsButton(mMainMenu);
	addTelevisionButton(mMainMenu);
	addEventsButton(mMainMenu);
	addFriendsButton(mMainMenu);
	addFriendListsButton(mMainMenu);
	addNotesButton(mMainMenu);
	addStatusMessagesButton(mMainMenu);
}

void FacebookDemoMoblet::uploadProfilePhoto(FacebookDemoGUI::ListScreen *menu)
{
	FacebookDemoApplication::AddCommand<Photo> *postPhotoCommand = new FacebookDemoApplication::AddCommand<Photo>(mFacebookManager, menu, "me");
	postPhotoCommand->setPhoto(mPicture, "Some message. Testing.");

	FacebookDemoGUI::ListItem *uploadButton = new FacebookDemoGUI::ListItem(NULL, "upload profile photo for user");
	menu->add(uploadButton);

}

void FacebookDemoMoblet::addLinkOnWall(FacebookDemoGUI::ListScreen *menu)
{

	FacebookDemoApplication::PostOnWallCommand<Link> *postLinkCommand = new FacebookDemoApplication::PostOnWallCommand<Link>(mFacebookManager, menu, "me");
	postLinkCommand->setLinkParams("http://www.youtube.com/watch?v=1JzFNaCT-KA", "Link posted with MOSYNC_SDK");

	FacebookDemoGUI::ListItem *postLinkButton = new FacebookDemoGUI::ListItem(postLinkCommand, "post a link on wall");
	menu->add(postLinkButton);
}

void FacebookDemoMoblet::addPostOnWall(FacebookDemoGUI::ListScreen *menu)
{
	FacebookDemoApplication::PostOnWallCommand<Post> *addPostCommand = new FacebookDemoApplication::PostOnWallCommand<Post>(mFacebookManager, menu, "me");
	addPostCommand->setPostParams(
		"Post added with MOSYN SDK",  					//message
		"http://www.youtube.com/watch?v=FL7yD-0pqZg", 	//link
		"",												//picture URL. You can't post both both a link and a picture URL => change FacebookManager->addPost
		"New post :)",									//name
		"Link from You Tube",							//caption
		"Testing adding a post on wall with MOSYNC_SDK");//description

	FacebookDemoGUI::ListItem *addPostButton = new FacebookDemoGUI::ListItem(addPostCommand, "add a post on wall");
	menu->add(addPostButton);
}

void FacebookDemoMoblet::addStatusMessageOnWall(FacebookDemoGUI::ListScreen *menu)
{
	FacebookDemoApplication::PostOnWallCommand<StatusMessage> *postStatusMessageCmd = new FacebookDemoApplication::PostOnWallCommand<StatusMessage>(mFacebookManager,
		menu, "me");
	postStatusMessageCmd->setMessage("Status message added on wall with MOSYNC_SDK");

	FacebookDemoGUI::ListItem *addStatusMessagButton = new FacebookDemoGUI::ListItem(postStatusMessageCmd, "add a status message on wall");
	menu->add(addStatusMessagButton);
}

void FacebookDemoMoblet::addActivitiesButton(FacebookDemoGUI::ListScreen *menu)
{

}

void FacebookDemoMoblet::addAlbumsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *albumsButtonCmd = new OpenMenuCommand(menu);

	//display the albums
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("name");
	fields.add("id");
	fields.add("description");
	fields.add("count");
	ConnectionRequestCommand *displayAlbumsCmd = new ConnectionRequestCommand(mFacebookManager,
		albumsButtonCmd->getMenuScreen(),
		Connections<User>::albums(), fields, "me");

	ListItem *displayAlbums = new ListItem(displayAlbumsCmd, "display albums");
	albumsButtonCmd->addMenuItem(displayAlbums);

	//create new album
	AddCommand<Album> *createAlbumCmd = new AddCommand<Album>(mFacebookManager, menu);
	createAlbumCmd->setAlbumName("New album created with MOSYNC_SDK");

	ListItem *createAlbum = new ListItem(createAlbumCmd, "create album");
	albumsButtonCmd->addMenuItem(createAlbum);

	ListItem *albumsButton = new ListItem(albumsButtonCmd, "albums");
	menu->add(albumsButton);
}

void FacebookDemoMoblet::addBooksButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayActivitiesCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::activities(), "me");

	ListItem *button = new ListItem(displayActivitiesCmd, "activities");
	menu->add(button);
}

void FacebookDemoMoblet::addCheckinsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *checkinsButtonCmd = new OpenMenuCommand(menu);

	//display the checkins
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("place");
	//fields.add("application");
	ConnectionRequestCommand *displayCheckinsCmd = new ConnectionRequestCommand( mFacebookManager,
		checkinsButtonCmd->getMenuScreen(), Connections<User>::checkins(), fields, "me");

	ListItem *displayCheckins = new ListItem(displayCheckinsCmd, "display checkins");
	checkinsButtonCmd->addMenuItem(displayCheckins);

	//create new checkin
	AddCommand<Checkin> *createCheckinCmd = new AddCommand<Checkin>( mFacebookManager, checkinsButtonCmd->getMenuScreen());
	MAUtil::String placeIdMoSync = "126381597427662";
	Coordinate coordMoSync;
	coordMoSync.mLatitude = "59.339451";
	coordMoSync.mLongitude = "18.05798";
	createCheckinCmd->setCheckinParams(placeIdMoSync, coordMoSync);

	ListItem *createCheckin = new ListItem(createCheckinCmd, "create checkin");
	checkinsButtonCmd->addMenuItem(createCheckin);

	ListItem *checkinsButton = new ListItem(checkinsButtonCmd, "checkins");
	menu->add(checkinsButton);
}

void FacebookDemoMoblet::addFeedButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("from");
	fields.add("name");
	fields.add("caption"); //"application" "type"
	ConnectionRequestCommand *displayFeedCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::feed(), fields, "me");

	ListItem *feedButton = new ListItem(displayFeedCmd, "feed");
	menu->add(feedButton);
}
void FacebookDemoMoblet::addHomeButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("from");
	fields.add("name");
	fields.add("caption"); //"application" "type"
	ConnectionRequestCommand *displayHomeCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::home(), fields, "me");

	ListItem *homeButton = new ListItem(displayHomeCmd, "home");
	menu->add(homeButton);
}
void FacebookDemoMoblet::addInterestsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayInterestsCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::interests(), "me");

	ListItem *button = new ListItem(displayInterestsCmd, "interests");
	menu->add(button);
}
void FacebookDemoMoblet::addLikesButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayLikesCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::likes(), "me");

	ListItem *button = new ListItem(displayLikesCmd, "likes");
	menu->add(button);
}
void FacebookDemoMoblet::addLinksButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("name");
	fields.add("message");
	ConnectionRequestCommand *displayLinksCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::links(), fields, "me");

	ListItem *button = new ListItem(displayLinksCmd, "links");
	menu->add(button);
}
void FacebookDemoMoblet::addMusicButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayMusicCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::music(), "me");

	ListItem *button = new ListItem(displayMusicCmd, "music");
	menu->add(button);
}
void FacebookDemoMoblet::addPhotosButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	MAUtil::Vector<MAUtil::String> fields;
	fields.add("from");
	fields.add("name");
	fields.add("id");
	ConnectionRequestCommand *displayPhotosCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::photos(), fields, "me");

	ListItem *button = new ListItem(displayPhotosCmd, "photos");
	menu->add(button);
}
void FacebookDemoMoblet::addPictureButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayPictureCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::picture(), "me");

	ListItem *button = new ListItem(displayPictureCmd, "picture");
	menu->add(button);
}
void FacebookDemoMoblet::addPostsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("from");
	fields.add("name");
	fields.add("caption"); //"application" "type"
	ConnectionRequestCommand *displayPostsCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::posts(), fields, "me");

	ListItem *button = new ListItem(displayPostsCmd, "posts");
	menu->add(button);
}
void FacebookDemoMoblet::addTelevisionButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayTelevisionCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::television(), "me");

	ListItem *button = new ListItem(displayTelevisionCmd, "television");
	menu->add(button);
}

void FacebookDemoMoblet::addEventsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *eventsButtonCmd = new OpenMenuCommand(menu);

	//display the events
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("name");
	fields.add("start_time");
	fields.add("location");
	ConnectionRequestCommand *displayEventsCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::events(), fields, "me");

	ListItem *displayEvents = new ListItem(displayEventsCmd, "Display events");
	eventsButtonCmd->addMenuItem(displayEvents);

	//create new event
	AddCommand<Event> *createEventCmd = new AddCommand<Event>( mFacebookManager, menu);

	//event
//	UnixTimeStamp startTimeStamp(Date("2012", "01", "10"), Time("8"));
//	UnixTimeStamp endTimeStamp(Date("2012", "02", "10"), Time("10"));

	UnixTimeStamp startTimeStamp(Date("2012", "10", "6"), Time("4","15","30"));
	UnixTimeStamp endTimeStamp(Date("2012", "10", "6"), Time("5", "20","00"));

	createEventCmd->setEventParams("New event created with MOSYNC_SDK", startTimeStamp, endTimeStamp,
		"Testing creating an event", "Stockholm");

	ListItem *createEvent = new ListItem(createEventCmd, "Create event");
	eventsButtonCmd->addMenuItem(createEvent);

	ListItem *eventsButton = new ListItem(eventsButtonCmd, "events");
	menu->add(eventsButton);
}

void FacebookDemoMoblet::addFriendsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	//display the friends
	ConnectionRequestCommand *displayFriendsCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::friends(), "me");

	ListItem *listsButton = new ListItem(displayFriendsCmd, "friends");
	menu->add(listsButton);
}

void FacebookDemoMoblet::addFriendListsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *listsButtonCmd = new OpenMenuCommand(menu);

	//display the friend lists
	ConnectionRequestCommand *displayListsCmd = new ConnectionRequestCommand( mFacebookManager, menu, Connections<User>::friendlists(), "me");
	ListItem *displayLists = new ListItem(displayListsCmd, "Display friend lists");
	listsButtonCmd->addMenuItem(displayLists);

	//create new friend list
	AddCommand<FriendList> *createListCmd = new AddCommand<FriendList>( mFacebookManager, menu );
	createListCmd->setFriendlistName("New friendlist");
	ListItem *createList = new ListItem(createListCmd, "Create friend list");
	listsButtonCmd->addMenuItem(createList);

	ListItem *listsButton = new ListItem(listsButtonCmd, "friendLists");
	menu->add(listsButton);
}

void FacebookDemoMoblet::addNotesButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *notesButtonCmd = new OpenMenuCommand(menu);

	//display the notes
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("subject");
	fields.add("from");
	ConnectionRequestCommand *displayNotesCmd = new ConnectionRequestCommand( mFacebookManager, menu, Connections<User>::notes(),
		fields, "me");
	ListItem *displayNotes = new ListItem(displayNotesCmd, "Display notes");
	notesButtonCmd->addMenuItem(displayNotes);

	//create new friend list
	AddCommand<Note> *createNoteCmd = new AddCommand<Note>( mFacebookManager, menu);
	createNoteCmd->setNoteParams("New note created with MOSYNC_SDK", "Testing creating a note with Facebook lib");
	ListItem *createNote = new ListItem(createNoteCmd, "Create Note");
	notesButtonCmd->addMenuItem(createNote);

	ListItem *listsButton = new ListItem(notesButtonCmd, "notes");
	menu->add(listsButton);
}

void FacebookDemoMoblet::addStatusMessagesButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *statusButtonCmd = new OpenMenuCommand(menu);

	//display status messages
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("message");
	ConnectionRequestCommand *displayStatusesCmd = new ConnectionRequestCommand( mFacebookManager,
		menu, Connections<User>::statuses(), fields, "me");
	ListItem *displayStatuses = new ListItem(displayStatusesCmd, "Display status messages");
	statusButtonCmd->addMenuItem(displayStatuses);

	//create new status message
	AddCommand<StatusMessage> *createStatusMsgCmd = new AddCommand<StatusMessage>( mFacebookManager, menu);
	createStatusMsgCmd->setMessage("New status message created with MOSYNC_SDK");
	ListItem *createStatusMsg = new ListItem(createStatusMsgCmd, "Create StatusMessage");
	statusButtonCmd->addMenuItem(createStatusMsg);

	ListItem *statusButton = new ListItem(statusButtonCmd, "status messages");
	menu->add(statusButton);
}
