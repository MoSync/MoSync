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

/*
 * FacebookMobletDemo.cpp
 */

#include <maapi.h>
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

#include <NativeUI/Screen.h>
#include <NativeUI/Image.h>

#include <matime.h>

/**
 * The constructor creates the user interface and the FacebookManager
 */
FacebookDemoMoblet::FacebookDemoMoblet(const MAUtil::String &appId):
	mImage(NULL), mImageSize(0)
{
	initializeFacebook(appId);
	createGUI();
	login();
}

void FacebookDemoMoblet::login()
{

	MAUtil::String oAuthUrl = mFacebookManager->getOAuthUrl();
	LOG("\t\tOAuthUrl = %s", oAuthUrl.c_str());

	mLoginScreen->setUrl(oAuthUrl);
	mLoginScreen->setListener(this);
	mLoginScreen->show();
}

void FacebookDemoMoblet::webViewHookInvoked( NativeUI::WebView* webView, int hookType,
		MAHandle urlData)
{

	int sz = maGetDataSize(urlData);

	if(sz<=0)
		return;

	char *newUrl = new char[sz];
	maReadData(urlData, newUrl, 0, sz);
	maDestroyObject(urlData);

	LOG("\t\ttnew URL: %s, sz=%d", newUrl, sz);

	MAUtil::String access_token = extractAccessToken(newUrl);
	if(access_token.size()>0)
	{
		mFacebookManager->setAccessToken(access_token);
		mMainScreen->show();
		mMainScreen->receiveKeyEvents(true);
	}

	delete []newUrl;

	LOG("\n\t\treceived access_token: %s", access_token.c_str());
}

void FacebookDemoMoblet::closeEvent()
{
	// Exit the app.
	close();
}

FacebookDemoMoblet::~FacebookDemoMoblet()
{
	// cleanup
	delete mFacebookManager;
	delete mLoginScreen;
	delete mMainScreen;
	delete []mImage;
}

/**
 * creates the FacebookManager object, that handles the requests to Facebook, retrieving data and publishing
 */
void FacebookDemoMoblet::initializeFacebook(const MAUtil::String &appId)
{
	MAUtil::Set<MAUtil::String> permissions;
	//ask for a access_token
	permissions.insert("offline_access");

	/**
	 * add to the "permissions" set the permission that we need for retrieving connections for
	 * a User and publishing on a User's wall
	 */
	GetPermissionsFor<User>::Retrieve::onlyConnections(permissions);
	GetPermissionsFor<User>::publishingData(permissions);


	/**
	 * add to the "permissions" set, the permissions that we need for retrieving an object, the connections for
	 * that object and for publishing a object
	 */
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

/**
 * creates the GUI
 */
void FacebookDemoMoblet::createGUI()
{
	using namespace FacebookDemoGUI;

	/**
	 * create the login screen and the main menu
	 */
	mLoginScreen = new FacebookLoginScreen();
	mMainScreen = new MainScreen(this);

	/**
	 * creation of buttons from the main menu
	 */

	//publish
//	uploadProfilePhoto(mMainScreen);
	addLinkOnWall(mMainScreen);
	addPostOnWall(mMainScreen);
	addStatusMessageOnWall(mMainScreen);

	//connections
	addActivitiesButton(mMainScreen);
	addAlbumsButton(mMainScreen);
	addBooksButton(mMainScreen);
	addCheckinsButton(mMainScreen);
	addFeedButton(mMainScreen);
	addHomeButton(mMainScreen);
	addInterestsButton(mMainScreen);
	addLikesButton(mMainScreen);
	addLinksButton(mMainScreen);
	addMusicButton(mMainScreen);
	addPhotosButton(mMainScreen);
//	addPictureButton(mMainScreen);

	addPostsButton(mMainScreen);
	addTelevisionButton(mMainScreen);
	addEventsButton(mMainScreen);
	addFriendsButton(mMainScreen);
	addFriendListsButton(mMainScreen);
	addNotesButton(mMainScreen);
	addStatusMessagesButton(mMainScreen);
}


MAUtil::String FacebookDemoMoblet::extractAccessToken(const char *newurl)
{
	MAUtil::String access_token;

//	const int BUFFER_SIZE = 16384;
//	char *newurl = new char[BUFFER_SIZE];
//	strcpy((char*) newurl, newUrl.c_str());


	if (strstr(newurl, "fbconnect://") == newurl)
	{
		Environment::getEnvironment().removeCustomEventListener(this);
		char *accessToken = strstr(newurl, "access_token=") + strlen("access_token=");
		char *expiresIn = strstr(newurl, "&expires_in=");
		accessToken[expiresIn - accessToken] = 0;
		access_token.append(accessToken, strlen(accessToken));
	}

	return access_token;
}

/**
 * creates a button and adds it to the main menu
 * adds on the button a command that sends the publish request to Facebook
 */
void FacebookDemoMoblet::uploadProfilePhoto(FacebookDemoGUI::ListScreen *menu)
{
	FacebookDemoApplication::AddCommand<Photo> *postPhotoCommand =
			new FacebookDemoApplication::AddCommand<Photo>(mFacebookManager, menu, "me");

	postPhotoCommand->setPhoto(mImage, mImageSize, "testing uploading a picture");

	FacebookDemoGUI::ListItem *uploadButton = new FacebookDemoGUI::ListItem(postPhotoCommand,
			"upload profile photo for user");
	menu->add(uploadButton);

}

/**
 * creates a button and adds it to the main menu
 * adds on the button a command that sends the upload picture request to Facebook
 */
void FacebookDemoMoblet::addLinkOnWall(FacebookDemoGUI::ListScreen *menu)
{

	FacebookDemoApplication::PostOnWallCommand<Link> *postLinkCommand = new FacebookDemoApplication::PostOnWallCommand<Link>(mFacebookManager, menu, "me");
	postLinkCommand->setLinkParams("http://www.youtube.com/watch?v=1JzFNaCT-KA", "Link posted with MOSYNC_SDK");

	FacebookDemoGUI::ListItem *postLinkButton = new FacebookDemoGUI::ListItem(postLinkCommand, "post a link on wall");
	menu->add(postLinkButton);
}

/**
 * creates a button and adds it to the main menu
 * adds on the button a command that sends the upload picture request to Facebook
 */
void FacebookDemoMoblet::addPostOnWall(FacebookDemoGUI::ListScreen *menu)
{
	FacebookDemoApplication::PostOnWallCommand<Post> *addLinkCommand = new FacebookDemoApplication::PostOnWallCommand<Post>(mFacebookManager, menu, "me");
	addLinkCommand->setPostParams(
			"Post added with MOSYN SDK", 										//message
			"http://www.youtube.com/watch?v=FL7yD-0pqZg",						//link
			"New post",															//name
			"Link from You Tube",												//caption
			"Testing adding a post on wall with MOSYNC_SDK");					//description

	FacebookDemoGUI::ListItem *linkButton = new FacebookDemoGUI::ListItem(addLinkCommand, "add a post on wall (video)");
	menu->add(linkButton);

	FacebookDemoApplication::PostOnWallCommand<Post> *addPictureCommand = new FacebookDemoApplication::PostOnWallCommand<Post>(mFacebookManager, menu, "me");
	addPictureCommand->setPostParams(
				"Post added with MOSYN SDK",																			//message
				"http://1.bp.blogspot.com/_8kXsPIEkJZI/RyHy_sITYCI/AAAAAAAAADc/4HTZMEhiVlw/s400/cats-in-love.jpg",		//link
				"New post",																								//name
				"Cats",																									//caption
				"Testing adding a post on wall with MOSYNC_SDK");														//description

	FacebookDemoGUI::ListItem *pictureButton = new FacebookDemoGUI::ListItem(addPictureCommand, "add a post on wall (picture)");
	menu->add(pictureButton);
}

/**
 * creates a button and adds it to the main menu
 * adds on the button a command that sends the upload picture request to Facebook
 */
void FacebookDemoMoblet::addStatusMessageOnWall(FacebookDemoGUI::ListScreen *menu)
{
	FacebookDemoApplication::PostOnWallCommand<StatusMessage> *postStatusMessageCmd = new FacebookDemoApplication::PostOnWallCommand<StatusMessage>(mFacebookManager,
			menu, "me");
	postStatusMessageCmd->setMessage("Status message added with MOSYNC_SDK");

	FacebookDemoGUI::ListItem *addStatusMessagButton = new FacebookDemoGUI::ListItem(postStatusMessageCmd, "add a status message on wall");
	menu->add(addStatusMessagButton);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addActivitiesButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *activitiesCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::activities(), "me");

	ListItem *button = new ListItem(activitiesCmd, "activities");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addAlbumsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *albumsButtonCmd = new OpenMenuCommand(menu);

	/**
	 * We only want to retrieve from Facebook the "name", "id", "description" and "count" fields from an Album object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("name");
	fields.add("id");
//	fields.add("description");
//	fields.add("count");
	ConnectionRequestCommand *displayAlbumsCmd = new ConnectionRequestCommand(  mFacebookManager,
			albumsButtonCmd->getMenuScreen(),
			Connections<User>::albums(), fields, "me");

	ListItem *displayAlbums = new ListItem(displayAlbumsCmd, "display albums");
	albumsButtonCmd->addMenuItem(displayAlbums);

	//create new album
	AddCommand<Album> *createAlbumCmd = new AddCommand<Album>( mFacebookManager,
			albumsButtonCmd->getMenuScreen());
	createAlbumCmd->setAlbumName("New album created with MOSYNC_SDK");

	ListItem *createAlbum = new ListItem(createAlbumCmd, "create album");
	albumsButtonCmd->addMenuItem(createAlbum);

	ListItem *albumsButton = new ListItem(albumsButtonCmd, "albums");
	menu->add(albumsButton);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addBooksButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayActivitiesCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::books(), "me");

	ListItem *button = new ListItem(displayActivitiesCmd, "books");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addCheckinsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *checkinsButtonCmd = new OpenMenuCommand(menu);

	/**
	 * We only want to retrieve from Facebook the "place" and "id" fields from an Checkin object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("place");
	//fields.add("application");
	ConnectionRequestCommand *displayCheckinsCmd = new ConnectionRequestCommand( mFacebookManager,
			checkinsButtonCmd->getMenuScreen(), Connections<User>::checkins(), fields, "me");

	ListItem *displayCheckins = new ListItem(displayCheckinsCmd, "display checkins");
	checkinsButtonCmd->addMenuItem(displayCheckins);

	//create new checkin
	AddCommand<Checkin> *createCheckinCmd = new AddCommand<Checkin>( mFacebookManager,
			checkinsButtonCmd->getMenuScreen());
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

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addFeedButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	/**
	 * We only want to retrieve from Facebook the "from", "id", "name" and "caption" fields from an Album object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("from");
	fields.add("name");
//	fields.add("caption");
	fields.add("message");
	fields.add("application");
	fields.add("type");
	ConnectionRequestCommand *displayFeedCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::feed(), fields, "me");

	ListItem *feedButton = new ListItem(displayFeedCmd, "feed");
	menu->add(feedButton);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addHomeButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	/**
	 * We only want to retrieve from Facebook the "id", "name", and "caption"  fields from an Album object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("from");
	fields.add("name");
//	fields.add("caption");
	fields.add("message");
	fields.add("application");
	fields.add("type");
	ConnectionRequestCommand *displayHomeCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::home(), fields, "me");

	ListItem *homeButton = new ListItem(displayHomeCmd, "home");
	menu->add(homeButton);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addInterestsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayInterestsCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::interests(), "me");

	ListItem *button = new ListItem(displayInterestsCmd, "interests");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addLikesButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayLikesCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::likes(), "me");

	ListItem *button = new ListItem(displayLikesCmd, "likes");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addLinksButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("name");
//	fields.add("message");
	ConnectionRequestCommand *displayLinksCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::links(), fields, "me");

	ListItem *button = new ListItem(displayLinksCmd, "links");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addMusicButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayMusicCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::music(), "me");

	ListItem *button = new ListItem(displayMusicCmd, "music");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addPhotosButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	/**
	 * We only want to retrieve from Facebook the "from", "name" and "id" fields from an Photo object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("from");
//	fields.add("name");
	fields.add("id");
	ConnectionRequestCommand *displayPhotosCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::photos(), fields, "me");

	ListItem *button = new ListItem(displayPhotosCmd, "photos");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addPictureButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayPictureCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::picture(), "me");

	ListItem *button = new ListItem(displayPictureCmd, "picture");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addPostsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	/**
	 * We only want to retrieve from Facebook the "id", "from", "name" and "caption" fields from an Album object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("from");
	fields.add("name");
//	fields.add("caption");
	fields.add("message");
	fields.add("application");
	fields.add("type");
	ConnectionRequestCommand *displayPostsCmd = new ConnectionRequestCommand( mFacebookManager, menu,
			Connections<User>::posts(), fields, "me");

	ListItem *button = new ListItem(displayPostsCmd, "posts");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addTelevisionButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	ConnectionRequestCommand *displayTelevisionCmd = new ConnectionRequestCommand( mFacebookManager, menu,
		Connections<User>::television(), "me");

	ListItem *button = new ListItem(displayTelevisionCmd, "television");
	menu->add(button);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addEventsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *eventsButtonCmd = new OpenMenuCommand(menu);

	/**
	 * We only want to retrieve from Facebook the "name", "id", "name" and "start_time" and "location" fields from an Event object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("name");
//	fields.add("start_time");
	fields.add("location");
	ConnectionRequestCommand *displayEventsCmd = new ConnectionRequestCommand( mFacebookManager,
			eventsButtonCmd->getMenuScreen(), Connections<User>::events(), fields, "me");

	ListItem *displayEvents = new ListItem(displayEventsCmd, "Display events");
	eventsButtonCmd->addMenuItem(displayEvents);

	//create new event
	AddCommand<Event> *createEventCmd = new AddCommand<Event>( mFacebookManager,
			eventsButtonCmd->getMenuScreen());

	//event
	UnixTimeStamp startTimeStamp(Date("2012", "10", "6"), Time("4","15","30"));
	UnixTimeStamp endTimeStamp(Date("2012", "10", "6"), Time("5", "20","00"));

	createEventCmd->setEventParams("New event created with MOSYNC_SDK", startTimeStamp, endTimeStamp,
			"Testing creating an event", "Stockholm");

	ListItem *createEvent = new ListItem(createEventCmd, "Create event");
	eventsButtonCmd->addMenuItem(createEvent);

	ListItem *eventsButton = new ListItem(eventsButtonCmd, "events");
	menu->add(eventsButton);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
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

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addFriendListsButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *listsButtonCmd = new OpenMenuCommand(menu);

	//display the friend lists
	ConnectionRequestCommand *displayListsCmd = new ConnectionRequestCommand( mFacebookManager,
			listsButtonCmd->getMenuScreen(),
			Connections<User>::friendlists(), "me");
	ListItem *displayLists = new ListItem(displayListsCmd, "Display friend lists");
	listsButtonCmd->addMenuItem(displayLists);

	//create new friend list
	tm tmTimeStamp;
	split_time(maLocalTime(), &tmTimeStamp);
	MAUtil::String strTimeStamp;
	strTimeStamp = MAUtil::integerToString(tmTimeStamp.tm_year + 1900);
	strTimeStamp += "/";
	strTimeStamp += MAUtil::integerToString(tmTimeStamp.tm_mon);
	strTimeStamp += "/";
	strTimeStamp += MAUtil::integerToString(tmTimeStamp.tm_mday);
	strTimeStamp += "_";
	strTimeStamp += MAUtil::integerToString(tmTimeStamp.tm_hour);
	strTimeStamp += ":";
	strTimeStamp += MAUtil::integerToString(tmTimeStamp.tm_min);
	strTimeStamp += ":";
	strTimeStamp += MAUtil::integerToString(tmTimeStamp.tm_sec);

//	strTimeStamp += MAUtil::integerToString(tmTimeStamp.tm_sec + rand());

	AddCommand<FriendList> *createListCmd = new AddCommand<FriendList>( mFacebookManager,
			listsButtonCmd->getMenuScreen() );
	createListCmd->setFriendlistName(strTimeStamp);

	ListItem *createList = new ListItem(createListCmd, "Create friend list");
	listsButtonCmd->addMenuItem(createList);

	ListItem *listsButton = new ListItem(listsButtonCmd, "friendLists");
	menu->add(listsButton);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addNotesButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *notesButtonCmd = new OpenMenuCommand(menu);

	/**
	 * We only want to retrieve from Facebook the "id", "subject" and "from" fields from an Album object
	 */
	MAUtil::Vector<MAUtil::String> fields;
	fields.add("id");
	fields.add("subject");
//	fields.add("from");
	ConnectionRequestCommand *displayNotesCmd = new ConnectionRequestCommand( mFacebookManager,
			notesButtonCmd->getMenuScreen(),
			Connections<User>::notes(), fields, "me");
	ListItem *displayNotes = new ListItem(displayNotesCmd, "Display notes");
	notesButtonCmd->addMenuItem(displayNotes);

	//create new friend list
	AddCommand<Note> *createNoteCmd = new AddCommand<Note>( mFacebookManager,
			notesButtonCmd->getMenuScreen());
	createNoteCmd->setNoteParams("New note created with MOSYNC_SDK", "Testing creating a note with Facebook library");
	ListItem *createNote = new ListItem(createNoteCmd, "Create Note");
	notesButtonCmd->addMenuItem(createNote);

	ListItem *listsButton = new ListItem(notesButtonCmd, "notes");
	menu->add(listsButton);
}

/**
 * Creates a button and adds it to the main menu
 * Adds on the button a command that sends the connection request to Facebook
 */
void FacebookDemoMoblet::addStatusMessagesButton(FacebookDemoGUI::ListScreen *menu)
{
	using namespace FacebookDemoGUI;
	using namespace FacebookDemoApplication;

	OpenMenuCommand *statusButtonCmd = new OpenMenuCommand(menu);

	/**
	 * We only want to retrieve from Facebook the "name", "id", "message" fields from an StatusMessage object
	 */
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
