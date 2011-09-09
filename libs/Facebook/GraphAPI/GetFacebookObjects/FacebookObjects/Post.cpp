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
 * Post.cpp
 */

#include "Post.h"

/**
 * Setter/getter for the "from" field of an Post object.
 * @param from - it represents the person/application that created the post. It contains an id and name.
 */
void Post::setFromfield(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Post::getFromField() const
{
	return mFrom;
}

/**
 * Setter/getter for the "to" field of an Post object.
 * @param to - profiles mentioned or targeted in this post. It contains an id and name.
 */
void Post::setToField(const IdNamePair &to)
{
	mTo = to;
}
const IdNamePair &Post::getToField() const
{
	return mTo;
}

/**
 * Setter/getter for the "message" field of an Post object.
 * @param message - the message of the post
 */
void Post::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Post::getMessage() const
{
	return mMessage;
}

/**
 * Setter/getter for the "picture" field of an Post object.
 * @param picture - a link to the picture included with this post
 */
void Post::setPictureUrl(const MAUtil::String &url)
{
	mPictureUrl = url;
}
const MAUtil::String &Post::getPictureUrl() const
{
	return mPictureUrl;
}

/**
 * Setter/getter for the "link" field of an Post object.
 * @param link - the link attached to this post
 */
void Post::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Post::getLink() const
{
	return mLink;
}

/**
 * Setter/getter for the "name" field of an Post object.
 * @param name - the name of the link
 */
void Post::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Post::getName() const
{
	return mName;
}

/**
 * Setter/getter for the "caption" field of an Post object.
 * @param caption - a description of the link (appears beneath the link caption)
 */
void Post::setCaption(const MAUtil::String &caption)
{
	mCaption = caption;
}
const MAUtil::String &Post::getCaption() const
{
	return mCaption;
}

/**
 * Setter/getter for the "description" field of an Post object.
 * @param desc - the description of the album
 */
void Post::setDescription(const MAUtil::String &desc)
{
	mDescription = desc;
}
const MAUtil::String &Post::getDescription() const
{
	return mDescription;
}

/**
 * Setter/getter for the "source" field of an Post object.
 * @param url - a URL to a Flash movie or video file to be embedded within the post
 */
void Post::setVideoUrl(const MAUtil::String &url)
{
	mVideoUrl = url;
}
const MAUtil::String &Post::getVideoUrl() const
{
	return mVideoUrl;
}

/**
 * Setter/getter for the "object_id" field of an Post object.
 * @param id - the Facebook object id for an uploaded photo or video
 */
void Post::setUploadedVideoOrPhotoId(const MAUtil::String &id)
{
	mUploadedFileId = id;
}
const MAUtil::String &Post::getUploadedVideoOrPhotoId() const
{
	return mUploadedFileId;
}

/**
 * Setter/getter for the "actions" field of an Post object.
 * @param actions - a Vector of available actions on the post
 * (including commenting, liking, and an optional app-specified action)
 */
void Post::setActions(const MAUtil::Vector<Action> &actions)
{
	mActions = actions;
}
const MAUtil::Vector<Action> &Post::getActions() const
{
	return mActions;
}

/**
 * Setter/getter for the "type" field of an Post object.
 * @param type - a string indicating the type for this post (including link, photo, video)
 */
void  Post::setType(const MAUtil::String &type)
{
	mType = type;
}
const MAUtil::String &Post::getType() const
{
	return mType;
}

/**
 * Setter/getter for the "likes" field of an Post object.
 * @param likes - likes for this post
 */
void Post::setLikes(const MAUtil::Vector<IdNamePair> &likes)
{
	mLikes = likes;
}
const MAUtil::Vector<IdNamePair> &Post::getLikes() const
{
	return mLikes;
}

/**
 * Setter/getter for the "comments" field of an Post object.
 * @param comments - comments for this post
 */
void Post::setComments(MAUtil::Vector<Comment> &comments)
{
	mComments = comments;
}
const MAUtil::Vector<Comment> &Post::getComments() const
{
	return mComments;
}

/**
 * Setter/getter for the "application" field of an Post object.
 * @param applicationInfo - information about the application this post came from
 */
void Post::setApplication(const IdNamePair &applicationInfo)
{
	mApplication = applicationInfo;
}
const IdNamePair &Post::getApplication() const
{
	return mApplication;
}
