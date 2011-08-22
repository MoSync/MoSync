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
 * PublishRequestCommand.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#ifndef PUBLISHREQUESTCOMMAND_H_
#define PUBLISHREQUESTCOMMAND_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <MAUtil/util.h>

#include "ICommand.h"
#include "../GUI/FacebookDemoScreen.h"
#include "../GUI/TextScreen.h"
#include "../GUI/ListItem.h"

#include <Facebook/FacebookManager.h>
#include <Facebook/GraphAPI/Publish/PublishListener.h>

#include <Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Utilities.h>
#include <Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Album.h>
#include <Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Checkin.h>
#include <Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/FriendList.h>
#include <Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/Note.h>
#include <Facebook/GraphAPI/GetFacebookObjects/FacebookObjects/StatusMessage.h>

#include <Facebook/LOG.h>

namespace FacebookDemoApplication
{

class PublishCommand: public FacebookDemoApplication::ICommand, public PublishingListener
{
public:
	PublishCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen)
	{
		mFacebookManager = facebookManager;
		mMessageScreen = new FacebookDemoGUI::TextScreen(prevScreen);
	}

	//FacebookPublisherListener2
	virtual void publishingResponseReceived(const MAUtil::String  &data, const MAUtil::String &path)
	{
		MAUtil::String message = "New object created. ID: ";
		message += data;
		mMessageScreen->setText(message);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

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

	virtual void queryError(int code, const MAUtil::String &path)
	{
		MAUtil::String message = "Publish request \"" + path + "\" failed.\nServer response: " +
			MAUtil::integerToString(code) + ".";

		mMessageScreen->setText(message);
		mMessageScreen->receiveKeyEvents(true);
		mMessageScreen->show();
	}

	virtual ~PublishCommand()
	{
		delete mMessageScreen;
	}
protected:
	void showPleaseWaitMessage()
	{
		mMessageScreen->receiveKeyEvents(false);
		mMessageScreen->setText("Please wait. Sending request...");
	}

protected:
	FacebookManager *mFacebookManager;
	FacebookDemoGUI::TextScreen *mMessageScreen;

};

class LikeCommand: public PublishCommand
{
public:
	LikeCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->Like(mId);
	}
private:
	MAUtil::String mId;
};

class UnlikeCommand: public PublishCommand
{
public:
	UnlikeCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->Unlike(mId);
	}
private:
	MAUtil::String mId;
};

template<class ObjectType>
class AddCommand;

template<>
class AddCommand<Album>: public PublishCommand
{

public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
		mName = "new album";
	}

	void setAlbumName(const MAUtil::String &name)
	{
		mName = name;
	}

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

template<>
class AddCommand<Checkin>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}

	void setCheckinParams(const MAUtil::String &placeId, const Coordinate &coord, const MAUtil::String &tags = "",  const MAUtil::String &message = "")
	{
		mPlaceId = placeId;
		mCoordinate = coord;
		mTags = tags;
		mMessage = message;
	}

	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addCheckin(mId, mPlaceId, mCoordinate, mTags, mMessage);
	}

private:
	MAUtil::String 	mId;
	MAUtil::String 	mPlaceId;
	Coordinate 		mCoordinate;
	MAUtil::String  mTags;
	MAUtil::String  mMessage;
};

template<>
class AddCommand<Event>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}
	void setEventParams(const MAUtil::String &eventName, const UnixTimeStamp &eventStart_time,  const UnixTimeStamp &eventEnd_time = UnixTimeStamp(),
				  const MAUtil::String &message="",      const MAUtil::String &location = "" ,const MAUtil::String &privacyType = "OPEN")
	{
		mEventName = eventName;
		mStartTime = eventStart_time;
		mEndTime = eventEnd_time;
		mMessage = message;
		mLocation = location;
		mPrivacy = privacyType;
	}
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


template<>
class AddCommand<EventResponse>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &eventId):
			PublishCommand(facebookManager, prevScreen)
	{
		mEventId = eventId;
		mResponse = maybe;

	}
	void setEventResponse(EVENT_RESPONSE_TYPE response)
	{
		mResponse = response;
	}
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->addEventResponse(mEventId, mResponse);
	}
private:
	MAUtil::String mEventId;
	EVENT_RESPONSE_TYPE mResponse;
};

template<>
class AddCommand<FriendList>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}
	void setFriendlistName(const MAUtil::String &name)
	{
		mName = name;
	}
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addFriendList(mId, mName);
	}

private:
	MAUtil::String mId;
	MAUtil::String mName;
};

template<>
class AddCommand<Note>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}
	void setNoteParams(const MAUtil::String &subject, const MAUtil::String &message="")
	{
		mSubject = subject;
		mMessage = message;
	}
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

template<>
class AddCommand<StatusMessage>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id = "me"):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}
	void setMessage(const MAUtil::String &message)
	{
		mMessage = message;
	}
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

template<>
class AddCommand<Comment>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &id):
		PublishCommand(facebookManager, prevScreen)
	{
		mId = id;
	}
	void setMessage(const MAUtil::String &message)
	{
		mMessage = message;
	}
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

template<>
class AddCommand<User>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
		PublishCommand(facebookManager, prevScreen)
	{
		mObjectId = targetObjectID;
	}
	void setUserId(const MAUtil::String &userId)
	{
		mUserId = userId;
	}
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

template<>
class AddCommand<Photo>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen,
			const MAUtil::String &targetObjectID):
		PublishCommand(facebookManager, prevScreen)
	{
		mObjectId = targetObjectID;
		mPixels = 0;
	}

	void setPhoto(const int *pixels, int pixelsArraySize, const MAUtil::String &message="")
	{
		mPixels = pixels;
		mPixelsArraySize = pixelsArraySize;
		mMessage = message;
	}

	void setPhoto(const MAUtil::String &picture, const MAUtil::String &message)
	{
		mPicture = picture;
		mMessage = message;
	}

	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		//mFacebookManager->addPhoto(mObjectId, mPixels, 36*36, mMessage);
		mFacebookManager->addPhoto(mObjectId, mPicture, mMessage);
	}

private:
	MAUtil::String 	mObjectId;	//album
	const int 	    *mPixels;
	int 			mPixelsArraySize;
	MAUtil::String 	mMessage;
	MAUtil::String  mPicture;
};

template<>
class AddCommand<Video>: public PublishCommand
{
public:
	AddCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
		PublishCommand(facebookManager, prevScreen)
	{
		mObjectId = targetObjectID;
	}

	void setVideo(int *videoSource, const MAUtil::String &videoTitle = "", const MAUtil::String &videoDescription = "")
	{
		mVideoSource = videoSource;
		mTitle = videoTitle;
		mDescription = videoDescription;
	}
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addVideo(mObjectId, mVideoSource, mTitle, mDescription);
	}

private:
	MAUtil::String 	mObjectId;	//album
	int 			*mVideoSource;
	MAUtil::String 	mTitle;
	MAUtil::String	mDescription;
};


template<class ObjectType>
class PostOnWallCommand;

template<>
class PostOnWallCommand<StatusMessage> :public PublishCommand
{
public:
	PostOnWallCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = targetObjectID;
	}

	void setMessage(const MAUtil::String &message)
	{
		mMessage = message;
	}
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
	PostOnWallCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = targetObjectID;
	}
	void setLinkParams(const MAUtil::String &link, const MAUtil::String &message ="")
	{
		mLink = link;
		mMessage = message;
	}
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

template<>
class PostOnWallCommand<Post> : public PublishCommand
{
public:
	PostOnWallCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen, const MAUtil::String &targetObjectID="me"):
			PublishCommand(facebookManager, prevScreen)
	{
		mId = targetObjectID;
	}
	//todo: modify params for adding a Post obj in all classes
	void setPostParams(const MAUtil::String &message, const MAUtil::String &link, const MAUtil::String &pictureUrl = "",
				  const MAUtil::String &name = "", const MAUtil::String &caption = "", const MAUtil::String &description = "",
				  const MAUtil::Vector<Action> &actions = MAUtil::Vector<Action>() )
	{
		mMessage = message;
		mLink = link;
		mPictureURL = pictureUrl;
		mName = name;
		mCaption = caption;
		mDescription = description;
		mActions = actions;
	}

	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->addPostOnWall(mId, mMessage, mLink, mPictureURL, mName, mCaption, mDescription, mActions);
	}
private:
	MAUtil::String mId;
	MAUtil::String mMessage;
	MAUtil::String mLink;
	MAUtil::String mPictureURL;
	MAUtil::String mName;
	MAUtil::String mCaption;
	MAUtil::String mDescription;
	MAUtil::Vector<Action> mActions;
};

template<class ObjectType>
class RemoveCommand;

template<>
class RemoveCommand<User> : public PublishCommand
{
public:
	RemoveCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *prevScreen,
			const MAUtil::String &targetObjectID, FacebookDemoGUI::ListItem *item) : PublishCommand(facebookManager, prevScreen)
	{
		mListId = targetObjectID;
		mDeletedItem = item;
	}
	void setUserId(const MAUtil::String &userId)
	{
		mUserId = userId;
	}
	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeUserFrom(mListId, mUserId);
	}
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
			//mMessageScreen->getPreviousScreen()->remove(mDeletedItem);
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

	virtual void execute()
	{

		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeComment(mCommentId);
	}

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
			//mMessageScreen->getPreviousScreen()->remove(mDeletedItem);
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

	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeFriendList(mFriendListId);
	}
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path)
	{
		if(mMessageScreen->getPreviousScreen())
		{
			//mMessageScreen->getPreviousScreen()->remove(mDeletedItem);
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

	virtual void execute()
	{
		showPleaseWaitMessage();
		mFacebookManager->setPublishingListener(this);
		mFacebookManager->removeComment(mNoteId);
	}
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path)
	{
		if(mMessageScreen->getPreviousScreen())
			{
				//mMessageScreen->getPreviousScreen()->remove(mDeletedItem);
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
