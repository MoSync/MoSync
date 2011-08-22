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
 * GraphAPI.cpp
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */


#include "FacebookObjectManager.h"

FacebookObjectManager::FacebookObjectManager( Facebook *facebook ):
	mFacebook(facebook), mListener(NULL)
{
}

FacebookObjectManager::~FacebookObjectManager()
{

}

void FacebookObjectManager::setListener(ObjectRequestListener *listener)
{
	mListener = listener;
}

void FacebookObjectManager::facebookObjectReceived(const Album &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Checkin &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Comment &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Event &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const FriendList &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Group &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Link &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Note &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Photo &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Page &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Post &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const StausMessage &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const User &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::facebookObjectReceived(const Video &object)
{
	delete mRequester;
	mListener->facebookObjectReceived(object);
}

void FacebookObjectManager::queryError(int code, const MAUtil::String &path)
{
	delete mRequester;
	mListener->queryError(code, path);
}
