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
