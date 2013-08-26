/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * FacebookObjectManager.cpp
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
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Checkin object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Checkin &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Comment object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Comment &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Event object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Event &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an FriendList object was received
 */
void FacebookObjectManager::facebookObjectReceived(const FriendList &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Group object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Group &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Link object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Link &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Note object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Note &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Photo object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Photo &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Page object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Page &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an Post object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Post &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an StausMessage object was received
 */
void FacebookObjectManager::facebookObjectReceived(const StausMessage &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * called when an User object was received
 */
void FacebookObjectManager::facebookObjectReceived(const User &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}
/*
 * called when an Video object was received
 */
void FacebookObjectManager::facebookObjectReceived(const Video &object)
{
	mListener->facebookObjectReceived(object);
	delete mRequester;
}

/*
 * Overwrite of ObjectRequestListener::queryError
 * @param code - HTTP error code received
 * @param id - the id of the object for that was requested from Facebook.
 */
void FacebookObjectManager::queryError(int code, const MAUtil::String &path)
{
	mListener->queryError(code, path);
	delete mRequester;
}
