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
 * ConnectionRequestCommand.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#ifndef CONNECTIONREQUESTCOMMAND_H_
#define CONNECTIONREQUESTCOMMAND_H_

#include <MAUtil/String.h>

#include "ICommand.h"
#include "OpenMenuCommand.h"

#include "../GUI/FacebookDemoScreen.h"
#include "../GUI/ListScreen.h"
#include "../GUI/TextScreen.h"
#include "../GUI/NativeUI/Screen.h"

#include <Facebook/GraphAPI/GetConnections/ConnectionsManagerListener.h>
#include <Facebook/FacebookManager.h>

class FacebookDemoGUI::ListItem;

namespace FacebookDemoApplication
{

class ConnectionRequestCommand: public FacebookDemoApplication::ICommand, public ConnectionsManagerListener
{

public:
	ConnectionRequestCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *previousScreen,
		const MAUtil::String &connection, const MAUtil::String &id);

	ConnectionRequestCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *previousScreen,
		const MAUtil::String &connection, const MAUtil::Vector<MAUtil::String> &fields, const MAUtil::String &id);

	//ConnectionsManagerListener
	virtual void received(const MAUtil::Vector<Photo> &photos, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<Post> &feed, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<Album> &albums, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<IdNamePair> &data, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<CategoryData> &likes, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<Note> &notes, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<Event> &events, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<Comment> &comments, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<Checkin> &checkins, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<EventResponse> &responses, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<Link> &links, const MAUtil::String &connType, const MAUtil::String &objectId);
	virtual void received(const MAUtil::Vector<StatusMessage> &messages, const MAUtil::String &connType,
		const MAUtil::String &objectId);

	virtual void errorReceivingConnecion(int code, const MAUtil::String &connType, const MAUtil::String &id); //todo

	virtual void execute();

	~ConnectionRequestCommand();

private:
	void createGUI(FacebookDemoGUI::FacebookDemoScreen *previousScreen);

	FacebookDemoGUI::ListItem *createItem(FacebookDemoApplication::ICommand *command, const MAUtil::String &itemText);

	void addToList(const Album &album);
	void addToList(const Comment &comment);
	void addToList(const Photo &photo);
	void addToList(const CategoryData &data, const MAUtil::String &connType, const MAUtil::String &objectId);
	void addToList(const IdNamePair &pair, const MAUtil::String &connType, const MAUtil::String &objectId);
	void addToList(const Post &post);
	void addToList(const Note &note);
	void addToList(const Event &event);
	void addToList(const Checkin &checkin);
	void addToList(const EventResponse &response);
	void addToList(const Link &link);
	void addToList(const StatusMessage &msg);

private:
	void createString(const Album &album, MAUtil::String &result);
	void createString(const Comment &comment, MAUtil::String &result);
	void createString(const Photo &comment, MAUtil::String &result);
	void createString(const CategoryData &data, MAUtil::String &result);
	void createString(const IdNamePair &pair, MAUtil::String &result);
	void createString(const Post &post, MAUtil::String &result);
	void createString(const Action &action, MAUtil::String &result, const MAUtil::String &prefix = "");
	void createString(const Note &note, MAUtil::String &result);
	void createString(const Event &event, MAUtil::String &result);
	void createString(const Venue &venue, MAUtil::String &result, const MAUtil::String &prefix = "");
	void createString(const Checkin &checkin, MAUtil::String &result);
	void createString(const Place &place, MAUtil::String &result, const MAUtil::String &prefix="");
	void createString(const EventResponse &response, MAUtil::String &result);
	void createString(const Link &response, MAUtil::String &result);
	void createString(const StatusMessage &msg, MAUtil::String &result);

	void addNewline(MAUtil::String &result);
private:
	void addConnection(FacebookDemoApplication::OpenMenuCommand *openMenuCmd,
		const MAUtil::String &connectionName, const MAUtil::String &id);
	void addLike(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);
	void addUnlike(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);
	void addComment(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);
	void addLinkOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);
	void addPostOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);
	void addStatusMessageOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);
	void addEventResponse(FacebookDemoApplication::OpenMenuCommand *openMenuCmd,
		const MAUtil::String &eventId, EVENT_RESPONSE_TYPE response);
	void removeUser(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &listId,
		const MAUtil::String &userId, FacebookDemoGUI::ListItem *item);
	void removeComment(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id, FacebookDemoGUI::ListItem *item);
	void removeFriendList(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id, FacebookDemoGUI::ListItem *item);
	void removeNote(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id, FacebookDemoGUI::ListItem *item);

private:
	FacebookDemoGUI::ListScreen *mResponseScreen;
	FacebookDemoGUI::TextScreen *mPleaseWaitScreen;

	FacebookManager *mFacebookManager;
	MAUtil::String mConnection;
	MAUtil::Vector<MAUtil::String> mFields;
	MAUtil::String mId;
};

}//namespace Application

#endif /* CONNECTIONREQUESTCOMMAND_H_ */
