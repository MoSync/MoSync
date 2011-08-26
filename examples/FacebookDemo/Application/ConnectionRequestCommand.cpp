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
 * ConnectionRequestCommand.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */
#include <MAUtil/util.h>

#include <Facebook/GraphAPI/GetConnections/Connections.h>

#include "ConnectionRequestCommand.h"
#include "PublishRequestCommand.h"

#include "../GUI/NativeUI/Button.h"
#include "../GUI/ListItem.h"
#include <Facebook/LOG.h>

namespace FacebookDemoApplication
{

ConnectionRequestCommand::ConnectionRequestCommand(FacebookManager *facebookManager,
		FacebookDemoGUI::FacebookDemoScreen *previousScreen, const MAUtil::String &connection,
		const MAUtil::String &id):
		mFacebookManager(facebookManager),
		mConnection(connection),
		mId(id)

{
	createGUI(previousScreen);
}

ConnectionRequestCommand::ConnectionRequestCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *previousScreen,
		const MAUtil::String &connection, const MAUtil::Vector<MAUtil::String> &fields, const MAUtil::String &id):
		mFacebookManager(facebookManager),
		mConnection(connection),
		mFields(fields),
		mId(id)
{
	createGUI(previousScreen);
}

//FacebookConnectionListener
void ConnectionRequestCommand::received(const MAUtil::Vector<Photo> &photos, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d photos.\n", photos.size());
	for(int i=0; i<photos.size(); i++)
	{
		addToList(photos[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<Post> &feed, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d posts.\n", feed.size());
	int maxItems = feed.size();
	if(feed.size()>8)
		maxItems = 8;
	for(int i=0; i<maxItems; i++)
	{
		addToList(feed[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<Album> &albums, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d albums.\n", albums.size());

	for(int i=0; i<albums.size(); i++)
	{
		addToList(albums[i]);
	}

	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<IdNamePair> &data, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d IdNamePairs.Connection: %s ", data.size(), connType.c_str());

	for(int i=0; i<data.size(); i++)
	{
		addToList(data[i], connType, objectId);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<CategoryData> &data, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d CategoryData objects. Connection: %s", data.size(), connType.c_str());

	for(int i=0; i<data.size(); i++)
	{
		addToList(data[i], connType, objectId);
	}

	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<Note> &notes, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d Notes.", notes.size());

	for(int i=0; i<notes.size(); i++)
	{
		addToList(notes[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<Event> &events, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d Events.", events.size());

	for(int i=0; i<events.size(); i++)
	{
		addToList(events[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<Comment> &comments, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d Comments.", comments.size());

	for(int i=0; i<comments.size(); i++)
	{
		addToList(comments[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<Checkin> &checkins, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d Checkins.", checkins.size());

	for(int i=0; i<checkins.size(); i++)
	{
		addToList(checkins[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<EventResponse> &responses, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d EventResponses.", responses.size());

	for(int i=0; i<responses.size(); i++)
	{
		addToList(responses[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<Link> &links, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d Links.", links.size());

	for(int i=0; i<links.size(); i++)
	{
		addToList(links[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::received(const MAUtil::Vector<StatusMessage> &messages, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	LOG("\n\t\tConnectionRequestCommand::received %d StatusMessages.", messages.size());

	for(int i=0; i<messages.size(); i++)
	{
		addToList(messages[i]);
	}
	mResponseScreen->show();
}

void ConnectionRequestCommand::errorReceivingConnecion(int code, const MAUtil::String &connType, const MAUtil::String &id)
{

	LOG("\n\t\tConnectionRequestCommand::errorReceivingConnecion. connection=%s, id=%s", connType.c_str(), id.c_str());

	MAUtil::String error = "Error retrieving connection " + connType +
							". Error code: " + MAUtil::integerToString(code);

	FacebookDemoGUI::ListItem *item = new FacebookDemoGUI::ListItem(NULL, error);
	mResponseScreen->add(item);

	mResponseScreen->show();
}

void ConnectionRequestCommand::execute()
{
	mPleaseWaitScreen->show();

	if(!mResponseScreen->isEmpty())
	{
		mResponseScreen->clear();
	}

	mFacebookManager->setConnectionRequestListener((ConnectionsManagerListener*)this);
	if(mFields.size()>0)
	{
		mFacebookManager->requestConnection(mConnection, mFields, mId);
	}
	else
	{
		mFacebookManager->requestConnection(mConnection, mId);
	}
}

ConnectionRequestCommand::~ConnectionRequestCommand()
{
	delete mResponseScreen;
}

void ConnectionRequestCommand::createGUI(FacebookDemoGUI::FacebookDemoScreen *previousScreen)
{
	mResponseScreen = new FacebookDemoGUI::ListScreen(previousScreen);
	mPleaseWaitScreen = new FacebookDemoGUI::TextScreen();
	mPleaseWaitScreen->setText("Sending request. Please wait...");
}

FacebookDemoGUI::ListItem *ConnectionRequestCommand::createItem(FacebookDemoApplication::ICommand *command,
		const MAUtil::String &itemText)
{
	FacebookDemoGUI::ListItem *item = new FacebookDemoGUI::ListItem(command, itemText);

	item->setProperty(MAW_WIDGET_ALPHA, (float)0.0);
	item->setFontColor(0xFFFFFF);

	item->setProperty(MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_LEFT);
	item->centerTextVertically();
	item->wrapContentVertically();

	return item;
}

void ConnectionRequestCommand::addToList(const Album &album)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	addConnection(openMenuCmd, Connections<Album>::photos(),   album.getId());
	addConnection(openMenuCmd, Connections<Album>::likes(), 	 album.getId());
	addConnection(openMenuCmd, Connections<Album>::comments(), album.getId());
	addConnection(openMenuCmd, Connections<Album>::picture() + MAUtil::String("(Android bug)"),  album.getId());

	addComment(openMenuCmd, album.getId());
	addLike(openMenuCmd, 	album.getId());
	addUnlike(openMenuCmd, 	album.getId());

	FacebookDemoGUI::ListItem *uploadPhoto = new FacebookDemoGUI::ListItem(NULL, "upload photo to this album");
	openMenuCmd->addMenuItem(uploadPhoto);

	MAUtil::String result;
	createString(album, result);

	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);
	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const Comment &comment)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	MAUtil::String result;
	createString(comment, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	addConnection(openMenuCmd, Connections<Comment>::likes(), comment.getId());
	addLike(openMenuCmd, 	comment.getId());
	addUnlike(openMenuCmd, 	comment.getId());
	removeComment(openMenuCmd, comment.getId(), item);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const Photo &photo)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	addConnection(openMenuCmd, Connections<Photo>::comments(), photo.getId());
	addConnection(openMenuCmd, Connections<Photo>::likes(),    photo.getId());
	addConnection(openMenuCmd, Connections<Photo>::picture(),  photo.getId());
	addConnection(openMenuCmd, Connections<Photo>::tags(), 	 photo.getId());

	addComment(openMenuCmd, photo.getId());
	addLike(openMenuCmd, 	photo.getId());
	addUnlike(openMenuCmd, 	photo.getId());

	MAUtil::String result;
	createString(photo, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const CategoryData &data, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	MAUtil::String result;
	createString(data, result);
	FacebookDemoGUI::ListItem *item = createItem(NULL, result);

	ICommand *itemCmd = NULL;
	if(connType == "friendlists")
	{
		OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

		addConnection(openMenuCmd, Connections<FriendList>::members(), data.mId);
		removeFriendList(openMenuCmd, data.mId, item);

		itemCmd = openMenuCmd;
	}
	item->setCommand(itemCmd);
	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const IdNamePair &pair, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	MAUtil::String result;
	createString(pair, result);
	FacebookDemoGUI::ListItem *item = createItem(NULL, result);

	ICommand *itemCmd = NULL;
	if( (connType == "members") || (connType == "friends") ) //retrieved Users
	{
		OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

		addConnection(openMenuCmd, Connections<User>::accounts(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::activities(), pair.mId);
		addConnection(openMenuCmd, Connections<User>::albums(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::books(), 		pair.mId);
		addConnection(openMenuCmd, Connections<User>::checkins(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::events(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::feed(), 		pair.mId);
		addConnection(openMenuCmd, Connections<User>::home(), 		pair.mId);
		addConnection(openMenuCmd, Connections<User>::interests(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::likes(), 		pair.mId);
		addConnection(openMenuCmd, Connections<User>::links(), 		pair.mId);
		addConnection(openMenuCmd, Connections<User>::music(), 		pair.mId);
		addConnection(openMenuCmd, Connections<User>::photos(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::picture(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::posts(), 		pair.mId);
		addConnection(openMenuCmd, Connections<User>::statuses(), 	pair.mId);
		addConnection(openMenuCmd, Connections<User>::television(), pair.mId);
		addConnection(openMenuCmd, Connections<User>::videos(), 	pair.mId);

		if((connType == "members"))
		{
			removeUser(openMenuCmd, objectId, pair.mId, item);
		}

		itemCmd = openMenuCmd;
	}

	item->setCommand(itemCmd);
	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const Post &post)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	addConnection(openMenuCmd, Connections<Post>::comments(), post.getId());
	addConnection(openMenuCmd, Connections<Post>::likes(),    post.getId());

	addComment(openMenuCmd, post.getId());
	addLike(openMenuCmd, 	post.getId());
	addUnlike(openMenuCmd, 	post.getId());

	MAUtil::String result;
	createString(post, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const Note &note)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	MAUtil::String result;
	createString(note, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	addConnection(openMenuCmd, Connections<Note>::comments(), note.getId());
	addConnection(openMenuCmd, Connections<Note>::likes(),    note.getId());

	addComment(openMenuCmd, note.getId());
	addLike(openMenuCmd, 	note.getId());
	addUnlike(openMenuCmd, 	note.getId());
	removeNote(openMenuCmd, note.getId(), item);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const Event &event)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	addConnection(openMenuCmd, Connections<Event>::feed(), 	  	event.getId());
	addConnection(openMenuCmd, Connections<Event>::noreply(),   event.getId());
	addConnection(openMenuCmd, Connections<Event>::maybe(), 	event.getId());
	addConnection(openMenuCmd, Connections<Event>::invited(),   event.getId());
	addConnection(openMenuCmd, Connections<Event>::attending(), event.getId());
	addConnection(openMenuCmd, Connections<Event>::declined(),  event.getId());
	addConnection(openMenuCmd, Connections<Event>::picture(),   event.getId());

	addStatusMessageOnWall(openMenuCmd, event.getId());
	addLinkOnWall(openMenuCmd, event.getId());
	addPostOnWall(openMenuCmd, event.getId());

	addEventResponse(openMenuCmd, event.getId(), maybe);
	addEventResponse(openMenuCmd, event.getId(), attending);
	addEventResponse(openMenuCmd, event.getId(), declined);

	MAUtil::String result;
	createString(event, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const Checkin &checkin)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	addConnection(openMenuCmd, Connections<Checkin>::comments(), checkin.getId());
	addConnection(openMenuCmd, Connections<Checkin>::likes(),    checkin.getId());
	addComment(openMenuCmd, checkin.getId());
	addLike(openMenuCmd, 	checkin.getId());
	addUnlike(openMenuCmd, 	checkin.getId());

	MAUtil::String result;
	createString(checkin, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const EventResponse &response)
{
	MAUtil::String result;
	createString(response, result);
	FacebookDemoGUI::ListItem *item = createItem(NULL, result);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const Link &link)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	addConnection(openMenuCmd, Connections<Link>::comments(), link.getId());
	addConnection(openMenuCmd, Connections<Link>::likes(), link.getId());
	addComment(openMenuCmd, link.getId());

	MAUtil::String result;
	createString(link, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::addToList(const StatusMessage &msg)
{
	OpenMenuCommand *openMenuCmd = new OpenMenuCommand(mResponseScreen);

	addConnection(openMenuCmd, Connections<StatusMessage>::comments(), msg.getId());
	addConnection(openMenuCmd, Connections<StatusMessage>::likes(),    msg.getId());
	addComment(openMenuCmd, msg.getId());
	addLike(openMenuCmd, 	msg.getId());
	addUnlike(openMenuCmd, 	msg.getId());

	MAUtil::String result;
	createString(msg, result);
	FacebookDemoGUI::ListItem *item = createItem(openMenuCmd, result);

	mResponseScreen->add(item);
}

void ConnectionRequestCommand::createString(const Album &album, MAUtil::String &result)
{
	result.clear();

	if(album.getName().size()>0)
	{
		result = "name: " + album.getName();
	}
	if(album.getId().size()>0)
	{
		addNewline(result);
		result += "id: " + album.getId();
	}
	if(album.getFromField().mName.size()>0)
	{
		addNewline(result);
		result += "from:     " + album.getFromField().mName;
		result += "\t\tid: " + album.getFromField().mId;
	}
	if(album.getDescription().size()>0)
	{
		addNewline(result);
		result += "description: " + album.getDescription();
	}
	if(album.getLink().size()>0)
	{
		addNewline(result);
		result += "link: " + album.getLink();
	}
	if(album.getCoverPhotoId().size()>0)
	{
		addNewline(result);
		result += "cover photo id: " + album.getCoverPhotoId();
	}
	if(album.getCount()>=0)
	{
		addNewline(result);
		result += "count: " + MAUtil::integerToString(album.getCount());
	}
}

void ConnectionRequestCommand::createString(const Comment &comment, MAUtil::String &result)
{
	result.clear();

	if(comment.getMessage().size()>0)
	{
		result = "message: " + comment.getMessage();
	}
	if(comment.getFromField().mName.size()>0)
	{
		addNewline(result);
		result += "from:     " + comment.getFromField().mName + "\n";
		result += "\t\tid: " + comment.getFromField().mId;
	}
	if(comment.getLikes()>=0)
	{
		addNewline(result);
		result += "likes: " + MAUtil::integerToString(comment.getLikes());
	}
}

void ConnectionRequestCommand::createString(const Photo &photo, MAUtil::String &result)
{
	result.clear();

	if(photo.getName().size()>0)
	{
		result = "name: " + photo.getName();
	}
	if(photo.getId().size()>0)
	{
		addNewline(result);
		result += "id: " + photo.getId();
	}
	if(photo.getFrom().mName.size()>0)
	{
		addNewline(result);
		result += "from: " + photo.getFrom().mName + "\n";
		result += "    id: " + photo.getFrom().mId;
	}

	if(photo.getSmallSizedPicture().size()>0)
	{
		addNewline(result);
		result = "thumbnail-sized: " + photo.getSmallSizedPicture();
	}

	if(photo.getFullSizedPicture().size()>0)
	{
		addNewline(result);
		result = "full-sized: " + photo.getFullSizedPicture();
	}

	if(photo.getLink().size()>0)
	{
		addNewline(result);
		result = "link: " + photo.getLink();
	}

	if(photo.getWidth()>=0)
	{
		addNewline(result);
		result += "width: " + MAUtil::integerToString(photo.getWidth());
	}

	if(photo.getHeight()>=0)
	{
		addNewline(result);
		result += "height: " + MAUtil::integerToString(photo.getHeight());
	}

	if(photo.getPositionInAlbum()>=0)
	{
		addNewline(result);
		result += "position in album: " + MAUtil::integerToString(photo.getPositionInAlbum());
	}
}

void ConnectionRequestCommand::createString(const CategoryData &data, MAUtil::String &result)
{
	result.clear();

	if(data.mName.size()>0)
	{
		result = "name: " + data.mName;
	}

	if(data.mId.size()>0)
	{
		addNewline(result);
		result += "id: " + data.mId;
	}

	if(data.mCategory.size()>0)
	{
		addNewline(result);
		result += "category: " + data.mCategory;
	}
	if(data.mDateCreated.size()>0)
	{
		addNewline(result);
		result += "date created: " + data.mDateCreated;
	}
}

void ConnectionRequestCommand::createString(const IdNamePair &pair, MAUtil::String &result)
{
	result.clear();

	if(pair.mName.size()>0)
	{
		result = "name: " + pair.mName;
	}

	if(pair.mId.size()>0)
	{
		addNewline(result);
		result+= "id:   " + pair.mId;
	}
}

void ConnectionRequestCommand::createString(const Post &post, MAUtil::String &result)
{
	result.clear();
	if(post.getName().size()>0)
	{
		result += "name: " + post.getName();
	}
	if(post.getId().size()>0)
	{
		addNewline(result);
		result += "id: " + post.getId();
	}
	if(post.getCaption().size()>0)
	{
		addNewline(result);
		result += "caption: " + post.getCaption();
	}
	if(post.getDescription().size()>0)
	{
		addNewline(result);
		result += "description: " + post.getDescription();
	}
	if(post.getMessage().size()>0)
	{
		addNewline(result);
		result += "message: " + post.getMessage();
	}
	if(post.getLink().size()>0)
	{
		addNewline(result);
		result += "link: " + post.getLink();
	}
	if(post.getToField().mName.size()>0)
	{
		addNewline(result);
		result += "to: " + post.getToField().mName;
		result += "   id: " + post.getToField().mId;
	}
	if(post.getPictureUrl().size()>0)
	{
		addNewline(result);
		result += "picture URL: " + post.getPictureUrl();
	}
	if(post.getVideoUrl().size()>0)
	{
		addNewline(result);
		result += "video URL: " + post.getVideoUrl();
	}
	if(post.getApplication().mName.size()>0)
	{
		addNewline(result);
		result += "application:  " + post.getApplication().mName;
		result += "\t\t\tid: " + post.getApplication().mId;
	}
	if(post.getComments().size()>0)
	{
		addNewline(result);
		result += "comments: \n";
		const MAUtil::Vector<Comment> &comments = post.getComments();
		for(int i=0; i<comments.size(); i++)
		{
			MAUtil::String temp;
			createString(comments[i], temp);
			result += "\t" + temp + "\n";
		}
	}
	if(post.getActions().size()>0)
	{
		addNewline(result);
		result += "actions: \n";
		const MAUtil::Vector<Action> &actions = post.getActions();
		for(int i=0; i<actions.size(); i++)
		{
			result += "\t\t" + actions[i].mName + "\n";
		}
	}
	if(post.getType().size()>0)
	{
		addNewline(result);
		result += "type: " + post.getType();
	}
	if(post.getFromField().mName.size()>0)
	{
		addNewline(result);
		result += "from:     " + post.getFromField().mName + "\n";
		result += "\t\tid: " + post.getFromField().mId;
	}
}

void ConnectionRequestCommand::createString(const Action &action, MAUtil::String &result,
		const MAUtil::String &prefix)
{
	result.clear();

	if(action.mName.size()>0)
	{
		result = prefix + "name: " + action.mName;
	}

	if(action.mLink.size()>0)
	{
		addNewline(result);
		result += prefix + "link: " + action.mLink;
	}
}

void ConnectionRequestCommand::createString(const Note &note, MAUtil::String &result)
{
	result.clear();

	if(note.getId().size()>0)
	{
		result += "id: " + note.getId();
	}
	if(note.getSubject().size()>0)
	{
		addNewline(result);
		result += "subject: " + note.getSubject();
	}
	if(note.getMessage().size()>0)
	{
		addNewline(result);
		result += "message: " + note.getMessage();
	}
	const MAUtil::Vector<Comment> &comments = note.getComments();
	if(comments.size()>0)
	{
		addNewline(result);
		result += "comments:\n";
		for(int i=0; i<comments.size(); i++)
		{
			MAUtil::String temp;
			createString(comments[i], temp);
			result += "\t" + temp + "\n";
		}
	}
	if(note.getFrom().mName.size()>0)
	{
		addNewline(result);
		result += "from:     " + note.getFrom().mName + "\n";
		result += "\t\tid: " + note.getFrom().mId;
	}
}

void ConnectionRequestCommand::createString(const Event &event, MAUtil::String &result)
{
	result.clear();

	if(event.getName().size()>0)
	{
		result = "name: " + event.getName();
	}
	if(event.getId().size()>0)
	{
		addNewline(result);
		result += "id: " + event.getId();
	}

	if(event.getStartTime().size()>0)
	{
		addNewline(result);
		result += "start time: " + event.getStartTime();
	}
	if(event.getEndTime().size()>0)
	{
		addNewline(result);
		result += "end time: " + event.getEndTime();
	}
	if(event.getOwner().mName.size()>0)
	{
		addNewline(result);
		result += "owner: " + event.getOwner().mName + "\n";
		result += "      id: " + event.getOwner().mId;
	}
	if(event.getDescription().size()>0)
	{
		addNewline(result);
		result += "description: " + event.getDescription();
	}
	if(event.getVenue().mCity.size()>0)
	{
		addNewline(result);
		result += "venue: \n";
		MAUtil::String temp;
		createString(event.getVenue(), temp);
		result += "\t" + temp;
	}
	if(event.getPrivacy().size()>0)
	{
		addNewline(result);
		result += "privacy: " + event.getPrivacy();
	}
	if(event.getLocation().size()>0)
	{
		addNewline(result);
		result += "location: " + event.getLocation();
	}
}

void ConnectionRequestCommand::createString(const Venue &venue, MAUtil::String &result, const MAUtil::String &prefix)
{
	result.clear();

	if(venue.mCountry.size()>0)
	{
		result += prefix + "country: " + venue.mCountry;
	}

	if(venue.mCity.size()>0)
	{
		addNewline(result);
		result += prefix + "city: " + venue.mCity;
	}

	if(venue.mState.size()>0)
	{
		addNewline(result);
		result += prefix + "state: " + venue.mState;
	}
	if(venue.mStreet.size()>0)
	{
		addNewline(result);
		result += prefix + "street: " + venue.mStreet;
	}
	if(venue.mZip.size()>0)
	{
		addNewline(result);
		result += prefix + "zip code: " + venue.mZip;
	}
	if(venue.mCoordinate.mLatitude.size()>0)
	{
		addNewline(result);
		result += prefix + "coordinate:\n\tlatitude: " + venue.mCoordinate.mLatitude + ", longitude: " +
				venue.mCoordinate.mLongitude;
	}
}

void ConnectionRequestCommand::createString(const Checkin &checkin, MAUtil::String &result)
{
	result.clear();

	if(checkin.getId().size()>0)
	{
		result += "id: " + checkin.getId();
	}
	if(checkin.getFrom().mName.size()>0)
	{
		addNewline(result);
		result += "from: " + checkin.getFrom().mName + "\n";
		result += "id:   " + checkin.getFrom().mId;
	}
	if(checkin.getMessage().size()>0)
	{
		addNewline(result);
		result += "message: " + checkin.getMessage() + "\n";
	}
	if(checkin.getPlace().mId.size()>0)
	{
		addNewline(result);
		result += "place:\n";
		MAUtil::String temp;
		createString(checkin.getPlace(), temp, "\t\t");
		result += temp;
	}
	if(checkin.getComments().size()>0)
	{
		addNewline(result);
		const MAUtil::Vector<Comment> &comments = checkin.getComments();
		result += "comments: ";
		for(int i=0; i<comments.size(); i++)
		{
			MAUtil::String temp;
			createString(comments[i], temp);
			result += "\t" + temp + "\n";
		}
	}
	if(checkin.getLikes().size()>0)
	{
		addNewline(result);
		const MAUtil::Vector<IdNamePair> &likes = checkin.getLikes();
		result += "likes: ";
		for(int i=0; i<likes.size(); i++)
		{
			MAUtil::String temp;
			createString(likes[i], temp);
			result += "\t" + temp + "\n";
		}
	}
	if(checkin.getTags().size()>0)
	{
		addNewline(result);
		const MAUtil::Vector<IdNamePair> &tags = checkin.getTags();
		result += "tags: ";
		for(int i=0; i<tags.size(); i++)
		{
			MAUtil::String temp;
			createString(tags[i], temp);
			result += "\t" + temp + "\n";
		}
	}
	if(checkin.getApplication().mId.size()>0)
	{
		addNewline(result);
		result += "application:  " + checkin.getApplication().mName + "\n";
		result += "\t\t\t id: " + checkin.getApplication().mId;
	}
}

void ConnectionRequestCommand::createString(const Place &place, MAUtil::String &result,
		const MAUtil::String &prefix)
{
	result.clear();
	if(place.mName.size()>0)
	{
		result += prefix + "name: " + place.mName;
	}
	if(place.mId.size()>0)
	{
		addNewline(result);
		result += prefix + "id: " + place.mId;
	}
	//result += "location:\n";
	if(place.mLocation.mState.size()>0)
	{
		addNewline(result);
		MAUtil::String temp;
		createString(place.mLocation, temp, prefix /*+ "\t\t"*/);
		result += temp;
	}
}

void ConnectionRequestCommand::createString(const EventResponse &response, MAUtil::String &result)
{
	result.clear();

	if(response.mId.size()>0)
	{
		result += "id: " + response.mId;
	}
	if(response.mName.size()>0)
	{
		addNewline(result);
		result += "name: " + response.mName;
	}

	if(response.mRsvp_Status.size()>0)
	{
		addNewline(result);
		result += "rsvp_status" + response.mRsvp_Status;
	}
}

void ConnectionRequestCommand::createString(const Link &link, MAUtil::String &result)
{
	result.clear();

	if(link.getId().size()>0)
	{
		result += "id: " + link.getId();
	}

	if(link.getName().size()>0)
	{
		addNewline(result);
		result += "name: " + link.getName();
	}

	if(link.getLink().size()>0)
	{
		addNewline(result);
		result += "link: " + link.getLink();
	}
	if(link.getFrom().mName.size()>0)
	{
		addNewline(result);
		result += "from: " + link.getFrom().mName + "\n";
		result += "     id: " + link.getFrom().mId;
	}

	if(link.getDescription().size()>0)
	{
		addNewline(result);
		result += "description: " + link.getDescription();
	}
	if(link.getPicture().size()>0)
	{
		addNewline(result);
		result += "picture: " + link.getPicture();
	}
	if(link.getMessage().size()>0)
	{
		addNewline(result);
		result += "message: " + link.getMessage();
	}
}

void ConnectionRequestCommand::createString(const StatusMessage &msg, MAUtil::String &result)
{
	result.clear();

	if(msg.getId().size()>0)
	{
		result += "id: " + msg.getId();
	}

	if(msg.getMessage().size()>0)
	{
		addNewline(result);
		result += "message: " + msg.getMessage();
	}

	if(msg.getFrom().mName.size()>0)
	{
		result += "\nfrom:     " + msg.getFrom().mName + "\n";
		result += "\t\tid: " + msg.getFrom().mId;
	}
}

void ConnectionRequestCommand::addNewline(MAUtil::String &result)
{
	if(result.size()>0 && result[result.size()-1] != '\n')
	{
		result += "\n";
	}
}

void ConnectionRequestCommand::addConnection(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &connectionName,
		const MAUtil::String &id)
{

	ConnectionRequestCommand *connectionRequest = new ConnectionRequestCommand( mFacebookManager,
																				openMenuCmd->getMenuScreen(),
																				connectionName,
																				id);

	FacebookDemoGUI::ListItem *connection = new FacebookDemoGUI::ListItem(connectionRequest, connectionName);
	openMenuCmd->addMenuItem(connection);
}

void ConnectionRequestCommand::addLike(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id)
{
	LikeCommand *likeCmd = new LikeCommand( mFacebookManager, openMenuCmd->getMenuScreen() , id);

	FacebookDemoGUI::ListItem *likeItem = new FacebookDemoGUI::ListItem(likeCmd, "like");
	openMenuCmd->addMenuItem(likeItem);
}

void ConnectionRequestCommand::addUnlike(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id)
{
	UnlikeCommand *unlikeCmd = new UnlikeCommand( mFacebookManager, openMenuCmd->getMenuScreen(), id);

	FacebookDemoGUI::ListItem *unlikeItem = new FacebookDemoGUI::ListItem(unlikeCmd, "unlike");
	openMenuCmd->addMenuItem(unlikeItem);
}

void ConnectionRequestCommand::addComment(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id)
{
	AddCommand<Comment> *createCommentCmd = new AddCommand<Comment>( mFacebookManager, openMenuCmd->getMenuScreen(), id);
	createCommentCmd->setMessage("Comment created with MOSYNC_SDK");

	FacebookDemoGUI::ListItem *createComment = new FacebookDemoGUI::ListItem(createCommentCmd, "create comment");
	openMenuCmd->addMenuItem(createComment);
}

void ConnectionRequestCommand::addLinkOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id)
{
	PostOnWallCommand<Link> *postLinkCommand = new PostOnWallCommand<Link>(mFacebookManager, openMenuCmd->getMenuScreen(), id);
	postLinkCommand->setLinkParams("http://www.youtube.com/watch?v=1JzFNaCT-KA", "Link posted with MOSYNC_SDK");

	FacebookDemoGUI::ListItem *postLinkButton = new FacebookDemoGUI::ListItem(postLinkCommand, "post a link on wall");
	openMenuCmd->addMenuItem(postLinkButton);
}

void ConnectionRequestCommand::addPostOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id)
{
	PostOnWallCommand<Post> *addPostCommand = new PostOnWallCommand<Post>(mFacebookManager, openMenuCmd->getMenuScreen(), id);

	addPostCommand->setPostParams(
				"Post added with MOSYN SDK",  					//message
				"http://www.youtube.com/watch?v=FL7yD-0pqZg", 	//link
				"",												//picture URL. You can't post both both a link and a picture URL => change FacebookManager->addPost
				"New Post object added with MOSYNC_SDK.",		//name
				"Link from You Tube",							//caption
				"Testing adding a post on wall with MOSYNC_SDK");//description


	FacebookDemoGUI::ListItem *addPostButton = new FacebookDemoGUI::ListItem(addPostCommand, "add a post on wall");
	openMenuCmd->addMenuItem(addPostButton);
}

void ConnectionRequestCommand::addStatusMessageOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd,
		const MAUtil::String &id)
{
	PostOnWallCommand<StatusMessage> *postStatusMessageCmd =
			new PostOnWallCommand<StatusMessage>(mFacebookManager, openMenuCmd->getMenuScreen(), id);

	postStatusMessageCmd->setMessage("Status message added with MOSYNC_SDK");

	FacebookDemoGUI::ListItem *addStatusMessagButton = new FacebookDemoGUI::ListItem(postStatusMessageCmd, "add a status message on wall");
	openMenuCmd->addMenuItem(addStatusMessagButton);
}

void ConnectionRequestCommand::addEventResponse(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &eventId,
		EVENT_RESPONSE_TYPE response)
{
	AddCommand<EventResponse> *addEventResponseCmd =
			new AddCommand<EventResponse>(mFacebookManager, openMenuCmd->getMenuScreen(), eventId);
	addEventResponseCmd->setEventResponse(response);

	MAUtil::String responseStr = "add ";
	switch(response)
	{
	case attending:
		responseStr +="\"attending\"";
		break;
	case declined:
		responseStr += "\"declined\"";
		break;
	case maybe:
		responseStr += "\"maybe\"";
		break;
	}

	responseStr += " response";

	FacebookDemoGUI::ListItem *addEventResponseButton = new FacebookDemoGUI::ListItem(addEventResponseCmd, responseStr);
	openMenuCmd->addMenuItem(addEventResponseButton);
}

void ConnectionRequestCommand::removeUser(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &listId,
		const MAUtil::String &userId, FacebookDemoGUI::ListItem *item)
{
	RemoveCommand<User> *removeUserCommand = new RemoveCommand<User>(mFacebookManager, openMenuCmd->getMenuScreen(), listId, item);
	removeUserCommand->setUserId(userId);

	FacebookDemoGUI::ListItem *removeUserButton = new FacebookDemoGUI::ListItem(removeUserCommand, "remove this user");
	openMenuCmd->addMenuItem(removeUserButton);
}

void ConnectionRequestCommand::removeComment(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id,
		FacebookDemoGUI::ListItem *item)
{
	RemoveCommand<Comment> *removeCommentCmd = new RemoveCommand<Comment>(mFacebookManager, openMenuCmd->getMenuScreen(), id, item);

	FacebookDemoGUI::ListItem *removeCommentButton = new FacebookDemoGUI::ListItem(removeCommentCmd, "remove this comment");
	openMenuCmd->addMenuItem(removeCommentButton);
}

void ConnectionRequestCommand::removeFriendList(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id,
		FacebookDemoGUI::ListItem *item)
{
	RemoveCommand<FriendList> *removeFriendListCmd = new RemoveCommand<FriendList>(mFacebookManager, openMenuCmd->getMenuScreen(), id, item);

	FacebookDemoGUI::ListItem *removeFriendListButton = new FacebookDemoGUI::ListItem(removeFriendListCmd, "remove this friend list");
	openMenuCmd->addMenuItem(removeFriendListButton);
}

void ConnectionRequestCommand::removeNote(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id,
		FacebookDemoGUI::ListItem *item)
{
	RemoveCommand<Note> *removeNoteCmd = new RemoveCommand<Note>(mFacebookManager, openMenuCmd->getMenuScreen(), id, item);

	FacebookDemoGUI::ListItem *removeNoteButton = new FacebookDemoGUI::ListItem(removeNoteCmd, "remove this note");
	openMenuCmd->addMenuItem(removeNoteButton);
}


}//namespace FacebookDemoApplication
