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
 * Link.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#include "Link.h"

void Link::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Link::getFrom() const
{
	return mFrom;
}

void Link::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Link::getLink() const
{
	return mLink;
}

void Link::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Link::getName() const
{
	return mName;
}

void Link::setComments(MAUtil::Vector<Comment> &comments)
{
	mComments.clear();
	mComments = comments;
}
const MAUtil::Vector<Comment> &Link::getComments() const
{
	return mComments;
}

void Link::setDescription(const MAUtil::String &descr)
{
	mDescription = descr;
}
const MAUtil::String &Link::getDescription() const
{
	return mDescription;
}

void Link::setPicture(const MAUtil::String &pictureUrl)
{
	mPicture = pictureUrl;
}
const MAUtil::String &Link::getPicture() const
{
	return mPicture;
}

void Link::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Link::getMessage() const
{
	return mMessage;
}
