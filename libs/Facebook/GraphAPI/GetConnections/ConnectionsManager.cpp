/*
 * ConnectionsManager.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: gabi
 */

#include "ConnectionsManager.h"
#include "../../JSON_lib/ParseJSONData.h"

#include "../../LOG.h"


using MAUtil::String;

/*
 * constructor.
 * @param facebook - pointer to a Facebook object. This object will actually make the request,
 * and will inform the ConnectionsManager when it retrieves a response from the server. If the
 * request is successful the JSON data will be passed to the ConnectionsManager. On failure the
 * ConnectionsManager is informed also. In this case the error code, requested connection and the
 * id of the object, for which the connection was made, are passed to the "ConnectionsManagerListener".
 */
ConnectionsManager::ConnectionsManager(Facebook *facebook):
				mFacebook(facebook),
				mConnectionListener(NULL),
				mMaxNumberOfObjectsToFetch(-1)
{
	mFacebook->setRetrieveDataListener((RetrieveDataListener*)this);

	/*
	 * mAvailableRequests - map that contains the mapping between the connection name ("feed", "home" ect.),
	 * and the function that parses the JSON data received from the server.
	 * Contains all the connection requests handled by the ConnectionsManager.
	 */
	/*
	 * The comments made on a Facebook object (Album, Post, Photo ect)
	 */
	mAvailableRequests["comments"]	 			= &ConnectionsManager::retrieveComments;

	/*
	 * User connections
	 */

	//The user's friends.
	mAvailableRequests["friends"] 				= &ConnectionsManager::retrieveIdNamePairs;

	//The user's news feed.
	mAvailableRequests["home"] 					= &ConnectionsManager::retrievePosts;

	//The user's wall.
	mAvailableRequests["feed"] 					= &ConnectionsManager::retrievePosts;

	//All the pages this user has liked.
	mAvailableRequests["likes"] 				= &ConnectionsManager::retrieveCategoryData;

	//The movies listed on the user's profile.
	mAvailableRequests["movies"] 				= &ConnectionsManager::retrieveCategoryData;

	//The music listed on the user's profile.
	mAvailableRequests["music"] 				= &ConnectionsManager::retrieveCategoryData;

	//The books listed on the user's profile.
	mAvailableRequests["books"] 				= &ConnectionsManager::retrieveCategoryData;

	//The user's notes.
	mAvailableRequests["notes"] 				= &ConnectionsManager::retrieveNotes;

	//The photo albums this user has created.
	mAvailableRequests["albums"] 				= &ConnectionsManager::retrieveAlbums;

	//The videos this user has been tagged in.
	mAvailableRequests["videos"] 				= &ConnectionsManager::retrieveVideos;

	//The videos this user has uploaded.
	mAvailableRequests["videos/uploaded"] 		= &ConnectionsManager::retrieveVideos;

	//The events this user is attending.
	mAvailableRequests["events"] 				= &ConnectionsManager::retrieveEvents;

	//The Groups that the user belongs to.
	mAvailableRequests["groups"] 				= &ConnectionsManager::retrieveGroups;

	//The places that the user has checked-into.
	mAvailableRequests["checkins"]	 			= &ConnectionsManager::retrieveCheckins;

	//The Facebook apps and pages owned by the current user.
	mAvailableRequests["accounts"]	 			= &ConnectionsManager::retrieveCategoryData;

	//The activities listed on the user's profile.
	mAvailableRequests["activities"]	 		= &ConnectionsManager::retrieveCategoryData;

	//The user's friend lists.
	mAvailableRequests["friendlists"]			= &ConnectionsManager::retrieveCategoryData;

	//The interests listed on the user's profile.
	mAvailableRequests["interests"]	 			= &ConnectionsManager::retrieveCategoryData;

	//The links listed on the user's profile.
	mAvailableRequests["links"]	 	 			= &ConnectionsManager::retrieveLinks;

	//The user's own posts.
	mAvailableRequests["posts"]	 	 			= &ConnectionsManager::retrievePosts;

	//The user's status updates.
	mAvailableRequests["statuses"]	 			= &ConnectionsManager::retrieveStatusMessages;

	//The television listed on the user's profile.
	mAvailableRequests["television"] 			= &ConnectionsManager::retrieveCategoryData;
	/*
	 * Photos the user (or friend) is tagged in
	 */
	mAvailableRequests["photos"] 				= &ConnectionsManager::retrievePhotos;

	/*
	 * Event connections
	 */

	//All of the users who have been not yet responded to their invitation to this event.
	mAvailableRequests["noreply"]	 			= &ConnectionsManager::retrieveEventResponses;

	//All of the users who have been responded "Maybe" to their invitation to this event.
	mAvailableRequests["maybe"]	 	 			= &ConnectionsManager::retrieveEventResponses;

	//All of the users who have been invited to this event.
	mAvailableRequests["invited"]	 	 		= &ConnectionsManager::retrieveEventResponses;

	//All of the users who have been responded "Attending" to their invitation to this event.
	mAvailableRequests["attending"]	 			= &ConnectionsManager::retrieveEventResponses;

	//All of the users who have been responded "Declined" to their invitation to this event.
	mAvailableRequests["declined"]	 			= &ConnectionsManager::retrieveEventResponses;


	/*
	 * Group and FriendList connections
	 */

	mAvailableRequests["members"]	 			= &ConnectionsManager::retrieveIdNamePairs;
	//"docs" connection: not implemented. TODO: implement

	/*
	 * Photo connections
	 */
	mAvailableRequests["tags"]	 	 			= &ConnectionsManager::retrieveIdNamePairs;
}

/*
 * Sets the listener of ConnectionsManager.
 * @param listener - the listener of the ConnectionsManager. It is informed by the result of the request.
 * On success a vector with the requested objects is passed to the listener.
 * On failure the error code, requested connection and the id of the object, for which the connection
 * was made will be send to the listener.
 */
void ConnectionsManager::setListener(ConnectionsManagerListener *listener)
{
	mConnectionListener = listener;
}

/* Sends the connection request.
 * @param connType - the connection type that is requested. (See http://developers.facebook.com/docs/reference/api/.)
 * @param id - the id of the object for which we request the connection. If the object is the user, nwe can use "me" as the id.
 * The request is made for all the fields of the object (id, name, tags ect). All the fields of the object will be retrieved
 * from the server.
 */
void ConnectionsManager::request(const MAUtil::String &connType, const MAUtil::String &id)
{
	mFields.clear();
	mFacebook->requestGraph(RETRIEVE_DATA, JSON, id + "/" + connType, HTTP_GET);
}

/* Sends the connection request.
 * @param connType - the connection type that is requested. (See http://developers.facebook.com/docs/reference/api/.)
 * @param id - the id of the object for which we request the connection. If the object is the user, nwe can use "me" as the id.
 * @param fields - specifies what fields of the object should be requested. For example we can request only for the
 * "id" and "name" fields.
 */
void ConnectionsManager::request(const MAUtil::String &connType, const MAUtil::Vector<MAUtil::String> &fields,
		const MAUtil::String &id)
{
	mFields.clear();
	MAUtil::Map<String, String> messageBodyParams;
	if(fields.size()>0)
	{
		MAUtil::String strFields;
		for(int i=0; i<fields.size(); ++i)
		{
			strFields += fields[i];
			if( (i + 1)<fields.size() )
			{
				strFields += ",";
			}
			mFields.insert(fields[i]);
		}
		messageBodyParams["fields="] = strFields;
	}
	mFacebook->requestGraph(RETRIEVE_DATA, JSON, id + "/" + connType, HTTP_GET, messageBodyParams);
}

void ConnectionsManager::limitTheNumberOfObjectsToFetch(int maxNumberOfObjects)
{
	mMaxNumberOfObjectsToFetch = maxNumberOfObjects;
}

void ConnectionsManager::fetchAllObjects()
{
	mMaxNumberOfObjectsToFetch = -1;
}

/*
 * This function is called when the "feed", "home"or "posts" connection is retrieved from server. It parses the
 * JSON data, and sends a vector of Post objects is send to the listener.
 */
void ConnectionsManager::retrievePosts(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<Post> posts;
	if(mFields.size()>0)
	{
		if(allObjectsMustBeFetched())
		{
			extract(posts, "data", result, mFields);
		}
		else
		{
			extract(posts, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if(allObjectsMustBeFetched())
		{
			extract(posts, "data", result);
		}
		else
		{
			extract(posts, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}
	mConnectionListener->received(posts, connType, objectId);
}

/*
 * This function is called when the "friends", "members" or "tags" connection is retrieved from server. It parses the
 * JSON data, and sends a vector of IdNamePair objects is send to the listener.
 */
void ConnectionsManager::retrieveIdNamePairs(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if( NULL == mConnectionListener )
	{
		return;
	}

	MAUtil::Vector<IdNamePair> friends;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched())
		{
			extract(friends, "data", result, mFields);
		}
		else
		{
			extract(friends, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched())
		{
			extract(friends, "data", result);
		}
		else
		{
			extract(friends, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}
	mConnectionListener->received(friends, connType, objectId);
}

/*
 * This function is called when the "likes", "movies", "music", "books" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of CategoryData objects is send to the listener.
 */
void ConnectionsManager::retrieveCategoryData(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<CategoryData> likes;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched())
		{
			extract(likes, "data", result, mFields);
		}
		else
		{
			extract(likes, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}

	}
	else
	{
		if( allObjectsMustBeFetched())
		{
			extract(likes, "data", result);
		}
		else
		{
			extract(likes, "data", result, mMaxNumberOfObjectsToFetch);
		}

	}
	mConnectionListener->received(likes, connType, objectId);
}

/*
 * This function is called when the "notes" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Note objects is send to the listener.
 */
void ConnectionsManager::retrieveNotes(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<Note> notes;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched() )
		{
			extract(notes, "data", result, mFields);
		}
		else
		{
			extract(notes, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}

	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(notes, "data", result);
		}
		else
		{
			extract(notes, "data", result, mMaxNumberOfObjectsToFetch);
		}

	}
	mConnectionListener->received(notes, connType, objectId);
}

/*
 * This function is called when the "photos" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Photo objects is send to the listener.
 */
void ConnectionsManager::retrievePhotos(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<Photo> photos;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched() )
		{
			extract(photos, "data", result, mFields);
		}
		else
		{
			extract(photos, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}

	}
	else
	{
		if( this->allObjectsMustBeFetched() )
		{
			extract(photos, "data", result);
		}
		else
		{
			extract(photos, "data", result, mMaxNumberOfObjectsToFetch);
		}

	}
	mConnectionListener->received(photos, connType, objectId);
}

/*
 * This function is called when the "albums" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Album objects is send to the listener.
 */
void ConnectionsManager::retrieveAlbums(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}

	//LOG("\t\tConnectionsManager::retrieveAlbums mFields.size()=%d", mFields.size());

	MAUtil::Vector<Album> albums;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched() )
		{
			extract(albums, "data", result, mFields);
		}
		else
		{
			extract(albums, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(albums, "data", result);
		}
		else
		{
			extract(albums, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}
	mConnectionListener->received(albums, connType, objectId);
}

/*
 * This function is called when the "videos" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Video objects is send to the listener.
 */
void ConnectionsManager::retrieveVideos(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<Video> videos;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched() )
		{
			extract(videos, "data", result, mFields);
		}
		else
		{
			extract(videos, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(videos, "data", result);
		}
		else
		{
			extract(videos, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}
	mConnectionListener->received(videos, connType, objectId);
}

/*
 * This function is called when the "events" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Event objects is send to the listener.
 */
void ConnectionsManager::retrieveEvents(YAJLDom::Value* result, const MAUtil::String &connTypes, const MAUtil::String &objectId)
{

	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<Event> events;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched())
		{
			extract(events, "data", result, mFields);
		}
		else
		{
			extract(events, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(events, "data", result);
		}
		else
		{
			extract(events, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}
	mConnectionListener->received(events, connTypes, objectId);
}

/*
 * This function is called when the "group" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Group objects is send to the listener.
 */
void ConnectionsManager::retrieveGroups(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<IdNamePair> groups;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched())
		{
			extract(groups, "data", result, mFields);
		}
		else
		{
			extract(groups, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(groups, "data", result);
		}
		else
		{
			extract(groups, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}
	mConnectionListener->received(groups, connType, objectId);
}

/*
 * This function is called when the "checkins" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Checkin objects is send to the listener.
 */
void ConnectionsManager::retrieveCheckins(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	MAUtil::Vector<Checkin> checkins;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched() )
		{
			extract(checkins, "data", result, mFields);
		}
		else
		{
			extract(checkins, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched())
		{
			extract(checkins, "data", result);
		}
		else
		{
			extract(checkins, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}
	mConnectionListener->received(checkins, connType, objectId);
}

/*
 * This function is called when the "comments" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Comment objects is send to the listener.
 */
void ConnectionsManager::retrieveComments(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if(NULL == mConnectionListener)
	{
		return;
	}
	MAUtil::Vector<Comment> comments;
	if(mFields.size()>0)
	{
		if( this->allObjectsMustBeFetched() )
		{
			extract(comments, "data", result, mFields);
		}
		else
		{
			extract(comments, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(comments, "data", result);
		}
		else
		{
			extract(comments, "data", result, mMaxNumberOfObjectsToFetch);
		}

	}
	mConnectionListener->received(comments, connType, objectId);
}

/*
 * This function is called when the "links" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of Link objects is send to the listener.
 */
void ConnectionsManager::retrieveLinks(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if((NULL == mConnectionListener) || (NULL == result))
	{
		return;
	}
	MAUtil::Vector<Link> links;
	if(mFields.size()>0)
	{
		if(allObjectsMustBeFetched())
		{
			extract(links, "data", result, mFields);
		}
		else
		{
			extract(links, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(links, "data", result);
		}
		else
		{
			extract(links, "data", result, mMaxNumberOfObjectsToFetch);
		}

	}
	mConnectionListener->received(links, connType, objectId);
}

/*
 * This function is called when the "noreply", "maybe", "attending" or "declined" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of EventResponse objects is send to the listener.
 */
void ConnectionsManager::retrieveEventResponses(YAJLDom::Value* result, const MAUtil::String &connType,
		const MAUtil::String &objectId)
{
	if((NULL == mConnectionListener) || (NULL == result))
	{
		return;
	}
	MAUtil::Vector<EventResponse> responses;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched() )
		{
			extract(responses, "data", result, mFields);
		}
		else
		{
			extract(responses, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(responses, "data", result);
		}
		else
		{
			extract(responses, "data", result, mMaxNumberOfObjectsToFetch);
		}
	}

	mConnectionListener->received(responses, connType, objectId);
}

/*
 * This function is called when the "statuses" connection is retrieved from server.
 * It parses the JSON data, and sends a vector of StatusMessage objects is send to the listener.
 */
void ConnectionsManager::retrieveStatusMessages(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	if((NULL == mConnectionListener) || (NULL == result))
	{
		return;
	}
	MAUtil::Vector<StatusMessage> messages;
	if(mFields.size()>0)
	{
		if( allObjectsMustBeFetched() )
		{
			extract(messages, "data", result, mFields);
		}
		else
		{
			extract(messages, "data", result, mFields, mMaxNumberOfObjectsToFetch);
		}
	}
	else
	{
		if( allObjectsMustBeFetched() )
		{
			extract(messages, "data", result);
		}
		else
		{
			extract(messages, "data", result, mMaxNumberOfObjectsToFetch);
		}

	}

	mConnectionListener->received(messages, connType, objectId);
}


/*
 * Overwrite of RetrieveDataListener::jsonDataReceived
 * @param result - the JSON data retrieved from Facebook, in response to the connection request
 * @param connectionType - the connection requested.
 */
void ConnectionsManager::jsonDataReceived(YAJLDom::Value* result, const MAUtil::String &connectionType,
		const MAUtil::String &objectId)
{
	if(mAvailableRequests.find(connectionType) != mAvailableRequests.end())
	{
		requestHandler function = mAvailableRequests[connectionType];
		(*this.*function)(result, connectionType, objectId);
	}
	else
	{
		MAUtil::String message = "Request \"" + connectionType + "\" not implemented!";
		maPanic(1, message.c_str());
	}
}

/*
 * This function is called when the "picture" connection is retrieved from server. The data retrieved
 * from the server is transformed into a image handle.
 */
void ConnectionsManager::imageReceived(MAHandle image, const MAUtil::String &connType, const MAUtil::String &objectId)
{
	mConnectionListener->received(image, connType, objectId);
}

/*
 * Overwrite of RetrieveDataListener::queryError
 * @param code - HTTP error code received
 * @param path - the id of the object for which the connection was requested, and the connection, in the form: id/connection
 * 				 e.g: me/feed, 575687658/picture
 */
void ConnectionsManager::queryError(int code, const MAUtil::String &path)
{
	LOG("\n\tConnectionsManager::queryError, mConnectionListener=%p", mConnectionListener);

	if(mConnectionListener)
	{
		MAUtil::String connType;
		MAUtil::String id;

		int found = path.findFirstOf('/');
		if(String::npos != found)
		{
			id = path.substr(0, found);
			connType = path.substr(found+1);
		}

		mConnectionListener->errorReceivingConnection(code, connType, id);
	}
}

/*
 * destructor.
 */
ConnectionsManager::~ConnectionsManager()
{
}

bool ConnectionsManager::allObjectsMustBeFetched()
{
	return ( mMaxNumberOfObjectsToFetch <= 0 );
}