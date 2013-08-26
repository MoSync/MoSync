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
 * Checkin.cpp
 */

#include "Checkin.h"

/**
 * Setter/getter for the "from" field of an Checkin object.
 * @param from - it represents the person/application that created the album. It contains an id and name.
 */
void Checkin::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Checkin::getFrom() const
{
	return mFrom;
}

/**
 * Setter/getter for the "tags" field of an Checkin object.
 * @param name - the users the author tagged in the checkin
 */
void Checkin::setTags(const MAUtil::Vector<IdNamePair> &tags)
{
	mTags = tags;
}
const MAUtil::Vector<IdNamePair> &Checkin::getTags() const
{
	return mTags;
}

/**
 * Setter/getter for the "place" field of an Checkin object.
 * @param place - information about the Facebook Page that represents the location of the checkin
 */
void Checkin::setPlace(const Place &place)
{
	mPlace = place;
}
const Place &Checkin::getPlace() const
{
	return mPlace;
}

/**
 * Setter/getter for the "likes" field of an Checkin object.
 * @param likes - users who like the checkin
 */
void Checkin::setLikes(const MAUtil::Vector<IdNamePair> &likes)
{
	mLikes = likes;
}
const MAUtil::Vector<IdNamePair> &Checkin::getLikes() const
{
	return mLikes;
}

/**
 * Setter/getter for the "message" field of an Checkin object.
 * @param message - the message the user added to the checkin
 */
void Checkin::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Checkin::getMessage() const
{
	return mMessage;
}

/**
 * Setter/getter for the "comments" field of an Checkin object.
 * @param comments - the message the user added to the checkin
 */
void Checkin::setComments(const MAUtil::Vector<Comment> &comments)
{
	mComments = comments;
}
const MAUtil::Vector<Comment> &Checkin::getComments() const
{
	return mComments;
}

/**
 * Setter/getter for the "app" field of an Checkin object.
 * @param app - information about the application that made the checkin
 */
void Checkin::setApplication(const IdNamePair &app)
{
	mApplication = app;
}
const IdNamePair &Checkin::getApplication() const
{
	return mApplication;
}
