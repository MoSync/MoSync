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
 * PhotoAlbum.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#include "Album.h"

Album::Album()
{
	mCount = -1;
}

void Album::setFromField(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Album::getFromField() const
{
	return mFrom;
}

void Album::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Album::getName() const
{
	return mName;
}

void Album::setDescription(const MAUtil::String &desc)
{
	mDescription = desc;
}
const MAUtil::String &Album::getDescription() const
{
	return mDescription;
}

void Album::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Album::getLink() const
{
	return mLink;
}

void Album::setCoverPhotoId(const MAUtil::String &id)
{
	mCoverPhotoId = id;
}
const MAUtil::String &Album::getCoverPhotoId() const
{
	return mCoverPhotoId;
}

void Album::setCount(int count)
{
	mCount = count;
}
int Album::getCount() const
{
	return mCount;
}
