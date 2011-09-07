/*
 * GraphAPI.cpp
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */


#include "FacebookObjectManager.h"

/*
 * constructor
 * @param facebook - pointer to a Facebook object. This object will actually make the request
 */
FacebookObjectManager::FacebookObjectManager( Facebook *facebook ):
	 mFacebook(facebook), mListener(0), mRequester(0)
{
}

/*
 * destructor
 */
FacebookObjectManager::~FacebookObjectManager()
{

}

/*
 * Sets the listener of FacebookObjectManager.
 * @param listener - the listener of the ObjectRequestListener. It is informed by the result of the request.
 * On success the facebookObjectReceived function is called and the object is send as a parameter.
 * On failure the error code and the id of the object, for which the request was made will be send to
 * the listener.
 */
void FacebookObjectManager::setListener(ObjectRequestListener *listener)
{
	mListener = listener;
}

/*
 * called when an Album object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Album &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Checkin object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Checkin &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Comment object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Comment &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Event object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Event &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an FriendList object was received
 */
void FacebookObjectManager::facebookObjectReceived(const FriendList &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Group object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Group &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Link object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Link &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Note object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Note &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Photo object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Photo &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Page object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Page &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an Post object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Post &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an StausMessage object was received
 */
void FacebookObjectManager::facebookObjectReceived(const StausMessage &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * called when an User object was received
 */
void FacebookObjectManager::facebookObjectReceived(const User &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}
/*
 * called when an Video object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Video &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

/*
 * Overwrite of ObjectRequestListener::queryError
 * @param code - HTTP error code received
 * @param id - the id of the object for that was requested from Facebook.
 */
void FacebookObjectManager::queryError(int code, const MAUtil::String &path)
{
	delete mRequester;
	mListener->queryError(code, path);
}
