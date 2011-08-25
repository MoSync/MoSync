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
 * Video.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#ifndef VIDEO_CPP_
#define VIDEO_CPP_

#include "Video.h"

void Video::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Video::getFrom() const
{
	return mFrom;
}

void Video::setTags(MAUtil::Vector<IdNamePair> &tags)
{
	mTags.clear();
	for(int i=0; i<tags.size(); i++)
	{
		mTags.add(tags[i]);
	}
}
const MAUtil::Vector<IdNamePair> &Video::getTags() const
{
	return mTags;
}

void Video::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Video::getName() const
{
	return mName;
}

void Video::setDescription(const MAUtil::String &descr)
{
	mDescription = descr;
}
const MAUtil::String &Video::getDescription() const
{
	return mDescription;
}

void Video::setPictureUrl(const MAUtil::String &url)
{
	mPictureUrl = url;
}
const MAUtil::String &Video::getPictureUrl()const
{
	return mPictureUrl;
}

void Video::setSource(const MAUtil::String &url)
{
	mSource = url;
}
const MAUtil::String &Video::getSource() const
{
	return mSource;
}

void Video::setComments(const MAUtil::Vector<Comment> &comments)
{
	mComments.clear();
	for(int i=0; i<comments.size(); i++)
	{
		mComments.add(comments[i]);
	}
}
const MAUtil::Vector<Comment> &Video::getComments() const
{
	return mComments;
}

#endif /* VIDEO_CPP_ */
