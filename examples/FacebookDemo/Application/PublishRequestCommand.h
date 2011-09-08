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
 * PublishRequestCommand.h
 */

#ifndef PUBLISHREQUESTCOMMAND_H_
#define PUBLISHREQUESTCOMMAND_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

#include "ICommand.h"
#include "../GUI/FacebookDemoScreen.h"
#include "../GUI/TextScreen.h"
#include "../GUI/ListItem.h"

#include "Facebook/FacebookManager.h"
#include "Facebook/GraphAPI/Publish/PublishListener.h"

#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Utilities.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Album.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Checkin.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/FriendList.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Note.h"
#include "Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/StatusMessage.h"

#include "Facebook/LOG.h"

#include <MAUtil/util.h>

namespace FacebookDemoApplication
{

/*
 * base class for all commands that publish on Facebook
 */
class PublishCommand: public FacebookDemoApplication::ICommand, public PublishingListener
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the publishing request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 */
	PublishCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen)
	{
		mFacebookManager = facebookManager;
		mMessageScreen = new FacebookDemoGUI::TextScreen(prevScreen);
	}

/*
	 * Called when the request was successful.
	 * @param newId - the id of the new object.
	 * @param path - contains the id of the object on which the publish request was made and the request name.
	 * 				  e.g: id/feed, id/likes
	 * This function is called when a new object is created (Album, Like, Comment, StatusMessage ect).
	 */
	virtual void publishingResponseReceived(const MAUtil::String  &newId, const MAUtil::String &path)
	{
		MAUtil::String message = "New object created. ID: ";
		message += newId;
		mMessageScreen->setText(message);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

	/*
	 * Called when the request was successful.
	 * @param success - the response from server, telling if the request was done.
	 * This function is called when a remove or unlike request was completed
	 */
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path)
	{
		MAUtil::String result;
		if(success)
		{
			result = "Publishing completed.";
		}
		else
		{
			result = "Publishing failed.";
		}
		mMessageScreen->setText(result);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

	/*
	 * Overwrite of RetrieveDataListener::queryError
	 * @param code - HTTP error code received
	 * @param path - the id of the object for which the connection was requested, and the connection, in the form: id/connection
	 * 				 e.g: me/feed, 575687658/picture
	 */
	virtual void queryError(int code, const MAUtil::String &path)
	{
		MAUtil::String message = "Publish request \"" + path + "\" failed.\nServer response: " +
				MAUtil::integerToString(code) + ".";

		mMessageScreen->setText(message);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

	/*
	 * destructor
	 * deletes the message screen
	 */
	virtual ~PublishCommand()
	{
		delete mMessageScreen;
	}
protected:
	/*
	 * Shows to the user a screen with the message ""Please wait. Sending request..."", after a publish request was made.
	 * The message is displayed until the request is completed.
	 */
	void showPleaseWaitMessage()
	{
		mMessageScreen->receiveKeyEvents(false);
		mMessageScreen->setText("Please wait. Sending request...");
	}

protected:
	/*
	 * The FacebookManager that will do the publish request
	 */
	FacebookManager *mFacebookManager;
	FacebookDemoGUI::TextScreen *mMessageScreen;

};

/*
 * Class used for posting a like to Facebook  (objectId/likes, HTTP_POST)
 */
class LikeCommand: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the like request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object to be liked (can be an Album, Comment, StatusMessage, Post ect)
	 */
	LikeCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->Like(mId);
	}
private:
	MAUtil::String mId;
};

/*
 * Class used for posting a unlike on Facebook  (objectId/likes, HTTP_DELETE)
 */
class UnlikeCommand: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the like request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object to be unliked (can be an Album, Comment, StatusMessage, Post ect)
	 */
	UnlikeCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->Unlike(mId);
	}
private:
	MAUtil::String mId;
};

/*
 * Class used for posting an object on Facebook. The template is specialized for all the Facebook
 * object types that we can post.
 */
template<class ObjectType>
class AddCommand;

/*
 * class for creating an Album for the user.
 */
template<>
class AddCommand<Album>: public PublishCommand
{

public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
		mName = "new album";
	}

	/*
	 * sets the "name" parameter of the request.
	 * @param name - the name of the album that will be created
	 */
	void setAlbumName(const MAUtil::String &name)
	{
		mName = name;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addAlbum(mId, mName);
	}

private:
	MAUtil::String mName;
	MAUtil::String mId;

};

/*
 * class for creating a Checkin for the user.
 */
template<>
class AddCommand<Checkin>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	/*
	 * sets the request parameters
	 */
	void setCheckinParams(const MAUtil::String &placeId, const Coordinate &coord, const MAUtil::String &tags = "",  const MAUtil::String &message = "")
	{
		mPlaceId = placeId;
		mCoordinate = coord;
		mTags = tags;
		mMessage = message;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addCheckin(mId, mPlaceId, mCoordinate, mTags, mMessage);
	}

private:
	MAUtil::String	mId;
	MAUtil::String	mPlaceId;
	Coordinate		mCoordinate;
	MAUtil::String	mTags;
	MAUtil::String	mMessage;
};

/*
 * class for creating a Event for a user.
 */
template<>
class AddCommand<Event>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	/*
	 * sets the request parameters
	 */
	void setEventParams(const MAUtil::String &eventName,	const UnixTimeStamp &eventStart_time,	const UnixTimeStamp &eventEnd_time = UnixTimeStamp(),
						const MAUtil::String &message="",	const MAUtil::String &location = "",	const MAUtil::String &privacyType = "OPEN")
	{
		mEventName = eventName;
		mStartTime = eventStart_time;
		mEndTime = eventEnd_time;
		mMessage = message;
		mLocation = location;
		mPrivacy = privacyType;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addEvent(mId, mEventName, mStartTime, mEndTime, mMessage, mLocation, mPrivacy);
	}

private:
	MAUtil::String mId;
	MAUtil::String mEventName;
	UnixTimeStamp  mStartTime;
	UnixTimeStamp  mEndTime;
	MAUtil::String mMessage;
	MAUtil::String mLocation;
	MAUtil::String mPrivacy;
};

/*
 * class for creating a EventResponse for a user.
 */
template<>
class AddCommand<EventResponse>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &eventId):
			PublishCommand(facebookManager, prevScreen)
	{
		mEventId = eventId;
		mResponse = maybe;

	}

	/*
	 * sets the request parameters
	 */
	void setEventResponse(EVENT_RESPONSE_TYPE response)
	{
		mResponse = response;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->addEventResponse(mEventId, mResponse);
	}
private:
	MAUtil::String mEventId;
	EVENT_RESPONSE_TYPE mResponse;
};

/*
 * class for creating an FriendList for a user.
 */
template<>
class AddCommand<FriendList>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen), mCreatedObjectsCount(0)
	{
		mId = id;
	}

	/*
	 * sets the request parameters
	 */
	void setFriendlistName(const MAUtil::String &name)
	{
		mName = name;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addFriendList(mId, mName + "_" + MAUtil::integerToString(mCreatedObjectsCount) );

		mCreatedObjectsCount++;
	}

private:
	MAUtil::String	mId;
	MAUtil::String	mName;
	int 			mCreatedObjectsCount;
};

/*
 * class for creating a Note for a user.
 */
template<>
class AddCommand<Note>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	/*
	 * sets the request parameters
	 */
	void setNoteParams(const MAUtil::String &subject, const MAUtil::String &message="")
	{
		mSubject = subject;
		mMessage = message;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addNote(mId, mSubject, mMessage);
	}

private:
	MAUtil::String mId;
	MAUtil::String mSubject;
	MAUtil::String mMessage;
};

/*
 * class for creating an StatusMessage for a user.
 */
template<>
class AddCommand<StatusMessage>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	/*
	 * sets the request parameters
	 */
	void setMessage(const MAUtil::String &message)
	{
		mMessage = message;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addStatusMessageOnWall(mId, mMessage);
	}

private:
	MAUtil::String mId;
	MAUtil::String mMessage;
};

/*
 * class for creating an Comment for a user.
 */
template<>
class AddCommand<Comment>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	/*
	 * sets the request parameters
	 */
	void setMessage(const MAUtil::String &message)
	{
		mMessage = message;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addComment(mId, mMessage);
	}

private:
	MAUtil::String mId;
	MAUtil::String mMessage;
};

/*
 * class for creating adding a User to a Group or Friendlist.
 */
template<>
class AddCommand<User>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
		PublishCommand(facebookManager, prevScreen)
	{
		mObjectId = targetObjectID;
	}

	/*
	 * sets the request parameters
	 */
	void setUserId(const MAUtil::String &userId)
	{
		mUserId = userId;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addUser(mObjectId, mUserId);
	}

private:
	MAUtil::String mObjectId;
	MAUtil::String mUserId;
};

/*
 * class for creating a Photo (to an Album or to the wall)
 */
template<>
class AddCommand<Photo>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen,
			const MAUtil::String &targetObjectID):
		PublishCommand(facebookManager, prevScreen)
	{
		mObjectId = targetObjectID;
		mPixels = 0;
	}

	/*
	 * sets the request parameters
	 */
	void setPhoto(const byte *pixels, int pixelsArraySize, const MAUtil::String &message="")
	{
		mPixels = pixels;
		mPixelsArraySize = pixelsArraySize;
		mMessage = message;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addPhoto(mObjectId, mPixels, mPixelsArraySize, mMessage);
	}

private:
	MAUtil::String	mObjectId;	//album
	const byte		*mPixels;
	int 			mPixelsArraySize;
	MAUtil::String	mMessage;
};

/*
 * class for posting a Video for a user.
 */
template<>
class AddCommand<Video>: public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param id - the id of the object on which the post is made
	 */
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
		PublishCommand(facebookManager, prevScreen)
	{
		mObjectId = targetObjectID;
	}

	/*
	 * sets the request parameters
	 */
	void setVideo(int *videoSource, const MAUtil::String &videoTitle = "", const MAUtil::String &videoDescription = "")
	{
		mVideoSource = videoSource;
		mTitle = videoTitle;
		mDescription = videoDescription;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addVideo(mObjectId, mVideoSource, mTitle, mDescription);
	}

private:
	MAUtil::String	mObjectId;
	int 			*mVideoSource;
	MAUtil::String	mTitle;
	MAUtil::String	mDescription;
};

/*
 * class for posting on a User or Event's wall.
 * We can post: a StatusMessage, Link or Post object
 */
template<class ObjectType>
class PostOnWallCommand;

/*
 * class for posting a StatusMessage object (on a User or a Event's wall).
 */
template<>
class PostOnWallCommand<StatusMessage> :public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param targetObjectID - the id of the object on which the post is made
	 */
	PostOnWallCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = targetObjectID;
	}

	/*
	 * sets the request parameters
	 */
	void setMessage(const MAUtil::String &message)
	{
		mMessage = message;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addStatusMessageOnWall(mId, mMessage);
	}
private:

	MAUtil::String mId;			//User, Event, Group
	MAUtil::String mMessage;
};

template<>
class PostOnWallCommand<Link> :public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param targetObjectID - the id of the object on which the post is made
	 */
	PostOnWallCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = targetObjectID;
	}

	/*
	 * sets the request parameters
	 */
	void setLinkParams(const MAUtil::String &link, const MAUtil::String &message ="")
	{
		mLink = link;
		mMessage = message;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addLinkOnWall(mId, mLink, mMessage);
	}
private:
	MAUtil::String mId;
	MAUtil::String mLink;
	MAUtil::String mMessage;
};

/*
 * class for posting a Post object (on a User or a Event's wall).
 */
template<>
class PostOnWallCommand<Post> : public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param targetObjectID - the id of the object on which the post is made
	 */
	PostOnWallCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID="me"):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = targetObjectID;
	}

	/*
	 * sets the request parameters
	 */
	void setPostParams(	const MAUtil::String &message,		const MAUtil::String &link,			const MAUtil::String &pictureUrl = "",
						const MAUtil::String &name = "",	const MAUtil::String &caption = "",	const MAUtil::String &description = "",
						const MAUtil::Vector<Action> &actions = MAUtil::Vector<Action>() )
	{
		mMessage = message;
		mLink = link;
		mName = name;
		mCaption = caption;
		mDescription = description;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addPostOnWall(mId, mMessage, mLink,mName, mCaption, mDescription);
	}
private:
	MAUtil::String mId;
	MAUtil::String mMessage;
	MAUtil::String mLink;
	MAUtil::String mName;
	MAUtil::String mCaption;
	MAUtil::String mDescription;
};

/*
 * class for removing an object from Facebook. It is specialized only for the objects that
 * can be removed.
 */
template<class ObjectType>
class RemoveCommand;

/*
 * class for removing a User (from a Group or Friendlist)
 */
template<>
class RemoveCommand<User> : public PublishCommand
{
public:
	/*
	 * constructor
	 * @param facebookManager - the FacebookManager object used to make the request
	 * @param prevScreen - the screen that will be shown when the user presses the back button
	 * @param targetObjectID - the id of the object on which the post is made
	 * @param item - the item from the ListScreen that we will remove, after the request is completed
	 */
	RemoveCommand(	FacebookManager *facebookManager,		FacebookDemoGUI::FacebookDemoScreen *prevScreen,
					const MAUtil::String &targetObjectID,	FacebookDemoGUI::ListItem *item) : PublishCommand(facebookManager, prevScreen)
	{
		mListId = targetObjectID;
		mDeletedItem = item;
	}

	/*
	 * sets the request parameters
	 */
	void setUserId(const MAUtil::String &userId)
	{
		mUserId = userId;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeUserFrom(mListId, mUserId);
	}

	/*
	 * called when the request was completed
	 * @param success - the response from server, telling if the request was done.
	 */
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path)
	{
		MAUtil::String result;
		if(success)
		{
			result = "Object removed.";
		}
		else
		{
			result = "Publishing failed.";
		}

		if(mMessageScreen->getPreviousScreen())
		{
			mMessageScreen->getPreviousScreen()->removeChild(mDeletedItem);
		}
		mMessageScreen->setText(result);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

private:
	MAUtil::String mListId;
	MAUtil::String mUserId;
	FacebookDemoGUI::ListItem *mDeletedItem;
};

/*
 * class for removing a Comment
 */
template<>
class RemoveCommand<Comment> : public PublishCommand
{
public:
	RemoveCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen,
			const MAUtil::String &targetObjectID, FacebookDemoGUI::ListItem *item) : PublishCommand(facebookManager, prevScreen)
	{
		mCommentId = targetObjectID;
		mDeletedItem = item;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{

		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeComment(mCommentId);
	}

	/*
	 * called when the request was completed
	 * @param success - the response from server, telling if the request was done.
	 */
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path)
	{
		MAUtil::String result;
		if(success)
		{
			result = "Object removed.";
		}
		else
		{
			result = "Publishing failed.";
		}

		if(mMessageScreen->getPreviousScreen())
		{
			mMessageScreen->getPreviousScreen()->removeChild(mDeletedItem);
		}
		mMessageScreen->setText(result);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

private:
	MAUtil::String mCommentId;
	FacebookDemoGUI::ListItem *mDeletedItem;
};

template<>
class RemoveCommand<FriendList> : public PublishCommand
{
public:
	RemoveCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen,
			const MAUtil::String &targetObjectID, FacebookDemoGUI::ListItem *item) : PublishCommand(facebookManager, prevScreen)
	{
		mFriendListId = targetObjectID;
		mDeletedItem = item;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeFriendList(mFriendListId);
	}

	/*
	 * called when the request was completed
	 * @param success - the response from server, telling if the request was done.
	 */
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path)
	{
		if(mMessageScreen->getPreviousScreen())
		{
			mMessageScreen->getPreviousScreen()->removeChild(mDeletedItem);
		}
		MAUtil::String result;
		if(success)
		{
			result = "Object removed.";
		}
		else
		{
			result = "Publishing failed.";
		}

		mMessageScreen->setText(result);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

private:
	MAUtil::String mFriendListId;
	FacebookDemoGUI::ListItem *mDeletedItem;
};

template<>
class RemoveCommand<Note> : public PublishCommand
{
public:
	RemoveCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen,
			const MAUtil::String &targetObjectID, FacebookDemoGUI::ListItem *item) : PublishCommand(facebookManager, prevScreen)
	{
		mNoteId = targetObjectID;
		mDeletedItem = item;
	}

	/*
	 * sends the request to Facebook
	 */
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeComment(mNoteId);
	}

	/*
	 * called when the request was completed
	 * @param success - the response from server, telling if the request was done.
	 */
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path)
	{
		if(mMessageScreen->getPreviousScreen())
			{
				mMessageScreen->getPreviousScreen()->removeChild(mDeletedItem);
			}

		MAUtil::String result;
		if(success)
		{
			result = "Object removed.";
		}
		else
		{
			result = "Publishing failed.";
		}

		mMessageScreen->setText(result);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

private:
	MAUtil::String mNoteId;
	FacebookDemoGUI::ListItem *mDeletedItem;
};

}//namespace FacebookDemoApplication

#endif /* PUBLISHREQUESTCOMMAND_H_ */
