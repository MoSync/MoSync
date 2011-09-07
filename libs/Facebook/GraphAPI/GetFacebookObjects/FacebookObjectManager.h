/*
 * GraphAPI.h
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */

#ifndef GRAPHAPI_H_
#define GRAPHAPI_H_

#include "../../JSON_lib/ParseJSONData.h"
#include "ObjectRequestListener.h"
#include "../RetrieveDataListener.h"
#include "FacebookObjects/FacebookObject.h"
#include "FacebookObjects/User.h"

#include "../../HTTP/Facebook.h"

/*
 * Base class for RequestHelper.
 */
struct RequestHelperBase: public RetrieveDataListener
{
	virtual void requestFacebookObject(const MAUtil::String &id) = 0;
	virtual void requestFacebookObject(const MAUtil::String &id,
			const MAUtil::Vector<MAUtil::String> &fields) = 0;
};

/*
 * Helper class for FacebookObjectManager
 * It is implemented to send a request for a given object type and then be deleted.
 * The template parameter "ObjectType" - the type of the object we are requesting.
 */
template<class ObjectType>
class RequestHelper: public RequestHelperBase
{
public:
	/*
	 * constructor
	 * It is implemented to send a request for a given object type and then be deleted.
	 * The template parameter "ObjectType" - the type of the object we are requesting.
	 * @param facebook - the object that performs the actually request
	 * @param listener - listener that will be notified when the request si completed or
	 * the request failed.
	 */
	RequestHelper(Facebook *facebook, ObjectRequestListener *listener): mFacebook(facebook), mListener(listener)
	{
		mFacebook->setRetrieveDataListener(this);
	}


	/*
	 * Function that request a Facebook object
	 * @param id - the id of the object that we want to retrieve from Facebook
	 */
	virtual void requestFacebookObject(const MAUtil::String &id)
	{
		//mFacebook->requestGraph(RETRIEVE_DATA, JSON, id + "?metadata=1", HTTP_GET);
		mFacebook->requestGraph(RETRIEVE_DATA, JSON, id, HTTP_GET);
	}

	/*
	 * Function that request a Facebook object
	 * @param id - the id of the object that we want to retrieve from Facebook
	 * @param fields - the properties of the object that we want to retrieve from server. For example
	 * we can ask only for name, or name and message.
	 */
	virtual void requestFacebookObject(const MAUtil::String &id,
			const MAUtil::Vector<MAUtil::String> &fields)
	{
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
			}
			messageBodyParams["fields="] = strFields;
		}
		mFacebook->requestGraph(RETRIEVE_DATA, JSON, id, HTTP_GET, messageBodyParams);
	}

	/*
	 * destructor
	 */
	~RequestHelper()
	{
	}


private:
	friend class Facebook;

	/*
	 * RetrieveDataListener overwrite
	 * It is called when a Facebook object is retrieved from server
	 */
	virtual void jsonDataReceived(YAJLDom::Value* result, const MAUtil::String &connType)
	{
		parseJSONData(mFacebookObject, result);
		mListener->facebookObjectReceived(mFacebookObject);
	}

	/*
	 * RetrieveDataListener overwrite
	 * It is called when a Facebook object request has failed.
	 * @param code - HTTP error code received from the server.
	 * @param id - the id of the object the was requested from Facebook.
	 */
	virtual void queryError(int code, const MAUtil::String &path)
	{
		if(mListener)
		{
			mListener->queryError(code, path);
		}
	}

public:
	ObjectType	mFacebookObject;
	Facebook	*mFacebook;
	ObjectRequestListener *mListener;
};


/*
 * Class that manages requesting and retrieving Facebook objects.
 * Every object in the social graph has a unique ID.
 * You can access the properties of an User object for example by calling
 * FacebookObjectManager::requestFacebookObject<User>(ID);
 */
class FacebookObjectManager: public ObjectRequestListener
{
public:
	/*
	 * constructor
	 * @param facebook - pointer to a Facebook object. This object will actually make the request
	 */
	FacebookObjectManager(Facebook *facebook);

	/*
	 * Template function that request a Facebook object
	 * @param id - the id of the object that we want to retrieve from Facebook
	 * The template parameter "Type" - is the type of the object we are requesting
	 * For all the types of Facebook objects that are implemented see:
	 * Facebook_lib/GraphAPI/GetFacebookObjects/FacebookObjects
	 */
	template<class Type>
	void requestFacebookObject(const MAUtil::String &id)
	{
		mRequester = new RequestHelper<Type>(mFacebook, this);
		mRequester->requestFacebookObject(id);
	}

	template<class Type>
	void requestFacebookObject(const MAUtil::String &id,
			const MAUtil::Vector<MAUtil::String> &fields)
	{
		mRequester = new RequestHelper<Type>(mFacebook, this);
		mRequester->requestFacebookObject(id, fields);
	}



	/*
	 * destructor
	 */
	~FacebookObjectManager();

	/*
	 * Sets the listener of FacebookObjectManager.
	 * @param listener - the listener of the ObjectRequestListener. It is informed by the result of the request.
	 * On success the facebookObjectReceived function is called and the object is send as a parameter.
	 * On failure the error code and the id of the object, for which the request was made will be send to
	 * the listener.
	 */
	void setListener(ObjectRequestListener *listener);

	/*
	 * called when an Album object was received
	 */
	virtual void facebookObjectReceived(const Album &object);
	/*
	 * called when an Checkin object was received
	 */
	virtual void facebookObjectReceived(const Checkin &object);
	/*
	 * called when an Comment object was received
	 */
	virtual void facebookObjectReceived(const Comment &object);
	/*
	 * called when an Event object was received
	 */
	virtual void facebookObjectReceived(const Event &object);
	/*
	 * called when an FriendList object was received
	 */
	virtual void facebookObjectReceived(const FriendList &object);
	/*
	 * called when an Group object was received
	 */
	virtual void facebookObjectReceived(const Group &object);
	/*
	 * called when an Link object was received
	 */
	virtual void facebookObjectReceived(const Link &object);
	/*
	 * called when an Note object was received
	 */
	virtual void facebookObjectReceived(const Note &object);
	/*
	 * called when an Photo object was received
	 */
	virtual void facebookObjectReceived(const Photo &object);
	/*
	 * called when an Page object was received
	 */
	virtual void facebookObjectReceived(const Page &object);
	/*
	 * called when an Post object was received
	 */
	virtual void facebookObjectReceived(const Post &object);
	/*
	 * called when an StausMessage object was received
	 */
	virtual void facebookObjectReceived(const StausMessage &object);
	/*
	 * called when an User object was received
	 */
	virtual void facebookObjectReceived(const User &object);
	/*
	 * called when an Video object was received
	 */
	virtual void facebookObjectReceived(const Video &object);

	/*
	 * Overwrite of ObjectRequestListener::queryError
	 * @param code - HTTP error code received
	 * @param id - the id of the object for that was requested from Facebook.
	 */
	virtual void queryError(int code, const MAUtil::String &id);

private:
	/*
	 * mFacebook - performs the actually connection request.
	 */
	Facebook					*mFacebook;

	/*
	 * mListener - pointer to a ObjectRequestListener. It will be notified by the result of the request.
	 * On success the retrieved object will be send to this listener (by calling his function facebookObjectReceived)
	 * On failure the HTTP error code and the id of the object that was requested is send to the listener.
	 */
	ObjectRequestListener		*mListener;

	/*
	 * mRequester - Pointer to an object that is created each time a request is made. The type of the object
	 * depends on the type of the requested  object.
	 */
	RequestHelperBase			*mRequester;
};


#endif /* GRAPHAPI_H_ */
