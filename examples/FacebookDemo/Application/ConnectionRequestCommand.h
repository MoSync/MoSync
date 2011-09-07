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
 * ConnectionRequestCommand.h
 */

#ifndef CONNECTIONREQUESTCOMMAND_H_
#define CONNECTIONREQUESTCOMMAND_H_

#include <MAUtil/String.h>

#include "ICommand.h"
#include "OpenMenuCommand.h"

#include "../GUI/FacebookDemoScreen.h"

#include "Facebook/GraphAPI/GetConnections/ConnectionsManagerListener.h"
#include "Facebook/FacebookManager.h"

#include "../GUI/TextScreen.h"
#include "../GUI/ImageScreen.h"
#include "../GUI/ListItem.h"

namespace FacebookDemoApplication
{

/*
 * class used to send connections request to Facebook. A FacebookManger object is used
 * to make the request.
 * We register as listeners, and will retrieve the data, after the request
 * is completed ("received" functions).
 */
class ConnectionRequestCommand: public FacebookDemoApplication::ICommand, public ConnectionsManagerListener
{

public:
	/*
	 * constructor
	 * @param facebookManager - the object that handles the requests to Facebook
	 * @param previousSCreen - the screen that will be shown when the user clicks the back button.
	 */
	ConnectionRequestCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *previousScreen,
			const MAUtil::String &connection, const MAUtil::String &id);

	/*
	 * constructor
	 * @param facebookManager - the object that handles the requests to Facebook
	 * @param previousSCreen - the screen that will be shown when the user clicks the back button.
	 * @param fields - the fields from the Facebook object that  we want to retrieve.
	 */
	ConnectionRequestCommand(FacebookManager *facebookManager, FacebookDemoGUI::FacebookDemoScreen *previousScreen,
			const MAUtil::String &connection, const MAUtil::Vector<MAUtil::String> &fields, const MAUtil::String &id);

	//*********************************************************************************************************************************
	/*
	 * ConnectionsManagerListener overrides
	 * These functions are called when the request is completed and sucessfull.
	 */

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
	/*
	 * This function is called when the "picture" connection is retrieved from server. The data retrieved
	 * from the server is transformed into a image handle.
	 */
	virtual void received(MAHandle image, const MAUtil::String &connType, const MAUtil::String &objectId);

	//*********************************************************************************************************************************

	/*
	 * This function is called when an error occurred.
	 */
	virtual void errorReceivingConnection(int code, const MAUtil::String &connType, const MAUtil::String &id); //todo

	/*
	 * sends the connection request to Facebook, using the FacebookManager object
	 */
	virtual void execute();

	~ConnectionRequestCommand();

private:
	/*
	 * creates the screen
	 */
	void createGUI(FacebookDemoGUI::FacebookDemoScreen *previousScreen);

	/*
	 * creates a list item (on which a Facebook object will be displayed)
	 */
	FacebookDemoGUI::ListItem *createItem(FacebookDemoApplication::ICommand *command, const MAUtil::String &itemText);

	//*********************************************************************************************************************************
	/*
	 * adds to the list a item that displays a Facebook object (can be a Album, Comment ect)
	 */
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
	//*********************************************************************************************************************************

private:
	//*********************************************************************************************************************************
	/*
	 * converts a Facebook object into a string that can be displayed to the user
	 */
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
	//*********************************************************************************************************************************

	/*
	 * adds a newline to the end of the string (if the newline isn't added already)
	 */
	void addNewline(MAUtil::String &result);

private:

	/*
	 * adds a connection request command on a button. Adds the button to a menu (the screen contained in openMenuCmd)
	 */
	void addConnection( FacebookDemoApplication::OpenMenuCommand *openMenuCmd,
						const MAUtil::String &connectionName,
						const MAUtil::String &id);
	void addConnection( FacebookDemoApplication::OpenMenuCommand *openMenuCmd,
						const MAUtil::String &connectionName,
						const MAUtil::Vector<MAUtil::String> &fields,
						const MAUtil::String &id);

	/*
	 * adds a like command on a button. Adds the button to a menu (the screen contained in openMenuCmd)
	 */
	void addLike(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);

	/*
	 * adds a unlike command on a button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void addUnlike(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);

	/*
	 * adds a comment button to a menu screen. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void addComment(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);

	/*
	 * adds a "add a link to wall"  command on a button.
	 */
	void addLinkOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);

	/*
	 * adds a "add post to wall"  command on a button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void addPostOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);

	/*
	 * adds a "add status message to wall"  request on a button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void addStatusMessageOnWall(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id);

	/*
	 * adds a set event response command on a button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void addEventResponse(FacebookDemoApplication::OpenMenuCommand *openMenuCmd,
						  const MAUtil::String &eventId,
						  EVENT_RESPONSE_TYPE response);

	/*
	 * adds a "remove User"  request on button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void removeUser(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &listId,
					const MAUtil::String &userId, FacebookDemoGUI::ListItem *item);

	/*
	 * adds a remove Comment command on a button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void removeComment(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id, FacebookDemoGUI::ListItem *item);

	/*
	 * adds a remove Friendlist command on a button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void removeFriendList(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id, FacebookDemoGUI::ListItem *item);

	/*
	 * adds a remove Note command on a button. Adds the button to a menu (the Screen contained in openMenuCmd)
	 */
	void removeNote(FacebookDemoApplication::OpenMenuCommand *openMenuCmd, const MAUtil::String &id, FacebookDemoGUI::ListItem *item);

private:
	const int mMaxEntriesInList;

	FacebookDemoGUI::ListScreen 	*mJsonResponseScreen;
	FacebookDemoGUI::ImageScreen 	*mImageResponseScreen;
	FacebookDemoGUI::TextScreen 	*mPleaseWaitScreen;

	FacebookManager					*mFacebookManager;
	MAUtil::String					mConnection;
	MAUtil::Vector<MAUtil::String> 	mFields;
	MAUtil::String  				mId;
};

}//namespace Application

#endif /* CONNECTIONREQUESTCOMMAND_H_ */
