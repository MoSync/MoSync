/*
 * ConnectionsManager.h
 *
 *  Created on: Jul 5, 2011
 *      Author: gabi
 */

#ifndef CONNECTIONSMANAGER_H_
#define CONNECTIONSMANAGER_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Vector.h>

#include "../../HTTP/Facebook.h"

#include "../RetrieveDataListener.h"
#include "ConnectionsManagerListener.h"

//see: http://developers.facebook.com/docs/reference/api/

/*
 * class that manages connections requests to Facebook. Also parses the data received
 * from server and saves it in a vector. If a "ConnectionsManagerListener" object is provided,
 * the vector with the requested data is send to this listener.
 */
class ConnectionsManager: public RetrieveDataListener
{

public:
	/*
	 * constructor.
	 * @param facebook - pointer to a Facebook object. This object will actually make the request,
	 * and will inform the ConnectionsManager when it retrieves a response from the server. If the
	 * request is successful the JSON data will be passed to the ConnectionsManager. On failure the
	 * ConnectionsManager is informed also. In this case the error code, requested connection and the
	 * id of the object, for which the connection was made, are passed to the "ConnectionsManagerListener".
	 */
	ConnectionsManager(Facebook *facebook);

	/*
	 * Sets the listener of ConnectionsManager.
	 * @param listener - the listener of the ConnectionsManager. It is informed by the result of the request.
	 * On success a vector with the requested objects is passed to the listener.
	 * On failure the error code, requested connection and the id of the object, for which the connection
	 * was made will be send to the listener.
	 */
	void setListener(ConnectionsManagerListener *listener);

	/* Sends the connection request.
	 * @param connType - the connection type that is requested. (See http://developers.facebook.com/docs/reference/api/.)
	 * @param id - the id of the object for which we request the connection. If the object is the user, nwe can use "me" as the id.
	 * The request is made for all the fields of the object (id, name, tags ect). All the fields of the object will be retrieved
	 * from the server.
	 */
	void request(const MAUtil::String &connType, const MAUtil::String &id = "me");

	/* Sends the connection request.
	 * @param connType - the connection type that is requested. (See http://developers.facebook.com/docs/reference/api/.)
	 * @param id - the id of the object for which we request the connection. If the object is the user, nwe can use "me" as the id.
	 * @param fields - specifies what fields of the object should be requested. For example we can request only for the
	 * "id" and "name" fields.
	 */
	void request(const MAUtil::String &connType, const MAUtil::Vector<MAUtil::String> &fields, const MAUtil::String &id = "me");

	void limitTheNumberOfObjectsToFetch(int maxNumberOfObjects);

	void fetchAllObjects();

	/*
	 * destructor.
	 */
	virtual ~ConnectionsManager();

private:

	bool allObjectsMustBeFetched();

	friend class Facebook;

	/*
	 * Overwrite of RetrieveDataListener::jsonDataReceived
	 * @param result - the JSON data retrieved from Facebook, in response to the connection request
	 * @param connectionType - the connection requested.
	 */
	virtual void jsonDataReceived(YAJLDom::Value* result, const MAUtil::String &connectionType,	const MAUtil::String &objectId);

	/*
	 * This function is called when the "picture" connection is retrieved from server. The data retrieved
	 * from the server is transformed into a image handle.
	 */
	virtual void imageReceived(MAHandle image, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * Overwrite of RetrieveDataListener::queryError
	 * @param code - HTTP error code received
	 * @param path - the id of the object for which the connection was requested, and the connection, in the form: id/connection
	 * 				 e.g: me/feed, 575687658/picture
	 */
	virtual void queryError(int code, const MAUtil::String &path);

protected:
	/*
	 * This function is called when the "feed", "home" or "posts" connection is retrieved from server. It parses the
	 * JSON data, and sends a vector of Post objects is send to the listener.
	 */
	void retrievePosts(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "friends", "members" or "tags" connection is retrieved from server. It parses the
	 * JSON data, and sends a vector of IdNamePair objects is send to the listener.
	 */
	void retrieveIdNamePairs(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "likes", "movies", "music", "books", "accounts", "activity"
	 * "friendlists" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of CategoryData objects is send to the listener.
	 */
	void retrieveCategoryData(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "notes" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Note objects is send to the listener.
	 */
	void retrieveNotes(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "photos" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Photo objects is send to the listener.
	 */
	void retrievePhotos(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "albums" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Album objects is send to the listener.
	 */
	void retrieveAlbums(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "videos" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Video objects is send to the listener.
	 */
	void retrieveVideos(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "events" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Event objects is send to the listener.
	 */
	void retrieveEvents(YAJLDom::Value* result, const MAUtil::String &connTypes, const MAUtil::String &objectId);

	/*
	 * This function is called when the "group" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Group objects is send to the listener.
	 */
	void retrieveGroups(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "checkins" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Checkin objects is send to the listener.
	 */
	void retrieveCheckins(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "comments" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Comment objects is send to the listener.
	 */
	void retrieveComments(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "links" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of Link objects is send to the listener.
	 */
	void retrieveLinks(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "noreply", "maybe", "attending" or "declined" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of EventResponse objects is send to the listener.
	 */
	void retrieveEventResponses(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

	/*
	 * This function is called when the "statuses" connection is retrieved from server.
	 * It parses the JSON data, and sends a vector of StatusMessage objects is send to the listener.
	 */
	void retrieveStatusMessages(YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId);

protected:
	/*
	 * mFacebook - performs the actually connection request.
	 */
	Facebook 					*mFacebook;

	/*
	 * ConnectionsManagerListener object - it is informed about the result of the request.
	 */
	ConnectionsManagerListener	*mConnectionListener;

	typedef void (ConnectionsManager::*requestHandler)(YAJLDom::Value*, const MAUtil::String &, const MAUtil::String &);

	/*
	 * mAvailableRequests - map that contains the mapping between the connection name ("feed", "home" ect.), and the function that
	 * parses the JSON data retrieved from the server.
	 * Contains all the connection requests handled by the ConnectionsManager.
	 */
	MAUtil::Map<MAUtil::String, requestHandler> mAvailableRequests;

	MAUtil::Set<MAUtil::String> mFields;

	int mMaxNumberOfObjectsToFetch;
};




#endif /* CONNECTIONSMANAGER_H_ */
