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
 * Post.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#include "Post.h"

void Post::setFromfield(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Post::getFromField() const
{
	return mFrom;
}

void Post::setToField(const IdNamePair &to)
{
	mTo = to;
}
const IdNamePair &Post::getToField() const
{
	return mTo;
}

void Post::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Post::getMessage() const
{
	return mMessage;
}

void Post::setPictureUrl(const MAUtil::String &url)
{
	mPictureUrl = url;
}
const MAUtil::String &Post::getPictureUrl() const
{
	return mPictureUrl;
}

void Post::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Post::getLink() const
{
	return mLink;
}

void Post::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Post::getName() const
{
	return mName;
}

void Post::setCaption(const MAUtil::String &caption)
{
	mCaption = caption;
}
const MAUtil::String &Post::getCaption() const
{
	return mCaption;
}

void Post::setDescription(const MAUtil::String &desc)
{
	mDescription = desc;
}
const MAUtil::String &Post::getDescription() const
{
	return mDescription;
}

void Post::setVideoUrl(const MAUtil::String &url)
{
	mVideoUrl = url;
}
const MAUtil::String &Post::getVideoUrl() const
{
	return mVideoUrl;
}

void Post::setUploadedVideoOrPhotoId(const MAUtil::String &id)
{
	mUploadedFileId = id;
}
const MAUtil::String &Post::getUploadedVideoOrPhotoId() const
{
	return mUploadedFileId;
}

void Post::setActions(const MAUtil::Vector<Action> &actions)
{
	mActions = actions;
}
const MAUtil::Vector<Action> &Post::getActions() const
{
	return mActions;
}

void  Post::setType(const MAUtil::String &type)
{
	mType = type;
}
const MAUtil::String &Post::getType() const
{
	return mType;
}

void Post::setLikes(const MAUtil::Vector<IdNamePair> &likes)
{
	mLikes = likes;
}
const MAUtil::Vector<IdNamePair> &Post::getLikes() const
{
	return mLikes;
}

void Post::setComments(MAUtil::Vector<Comment> &comments)
{
	mComments = comments;
}
const MAUtil::Vector<Comment> &Post::getComments() const
{
	return mComments;
}

void Post::setApplication(const IdNamePair &applicationInfo)
{
	mApplication = applicationInfo;
}
const IdNamePair &Post::getApplication() const
{
	return mApplication;
}
