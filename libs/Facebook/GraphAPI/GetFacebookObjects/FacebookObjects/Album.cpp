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
 * Album.cpp
 */

#include "Album.h"

Album::Album()
{
	mCount = -1;
}

/**
 * Setter/getter for the "from" field of an Album object.
 * @param from - it represents the person/application that created the album. It contains an id and name.
 */
void Album::setFromField(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Album::getFromField() const
{
	return mFrom;
}


/**
 * Setter/getter for the "name" field of an Album object.
 * @param name - the title of the album
 */
void Album::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Album::getName() const
{
	return mName;
}

/**
 * Setter/getter for the "description" field of an Album object.
 * @param desc - the description of the album
 */
void Album::setDescription(const MAUtil::String &desc)
{
	mDescription = desc;
}
const MAUtil::String &Album::getDescription() const
{
	return mDescription;
}

/**
 * Setter/getter for the "link" field of an Album object.
 * @param link - a link to this album on Facebook
 */
void Album::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Album::getLink() const
{
	return mLink;
}

/**
 * Setter/getter for the "cover_photo" field of an Album object.
 * @param id - the album cover photo ID.
 */
void Album::setCoverPhotoId(const MAUtil::String &id)
{
	mCoverPhotoId = id;
}
const MAUtil::String &Album::getCoverPhotoId() const
{
	return mCoverPhotoId;
}

/**
 * Setter/getter for count.
 * @param count - the number of photos in this album.
 */
void Album::setCount(int count)
{
	mCount = count;
}
int Album::getCount() const
{
	return mCount;
}
