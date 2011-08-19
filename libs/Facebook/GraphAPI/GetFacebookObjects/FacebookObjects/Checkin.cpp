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
 * Checkin.cpp
 *
 *  Created on: Jul 8, 2011
 *      Author: gabi
 */

#include "Checkin.h"

void Checkin::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Checkin::getFrom() const
{
	return mFrom;
}

void Checkin::setTags(const MAUtil::Vector<IdNamePair> &tags)
{
	mTags = tags;
}
const MAUtil::Vector<IdNamePair> &Checkin::getTags() const
{
	return mTags;
}

void Checkin::setPlace(const Place &place)
{
	mPlace = place;
}
const Place &Checkin::getPlace() const
{
	return mPlace;
}

void Checkin::setLikes(const MAUtil::Vector<IdNamePair> &likes)
{
	mLikes = likes;
}
const MAUtil::Vector<IdNamePair> &Checkin::getLikes() const
{
	return mLikes;
}

void Checkin::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Checkin::getMessage() const
{
	return mMessage;
}

void Checkin::setComments(const MAUtil::Vector<Comment> &comments)
{
	mComments = comments;
}
const MAUtil::Vector<Comment> &Checkin::getComments() const
{
	return mComments;
}

void Checkin::setApplication(const IdNamePair &app)
{
	mApplication = app;
}
const IdNamePair &Checkin::getApplication() const
{
	return mApplication;
}
