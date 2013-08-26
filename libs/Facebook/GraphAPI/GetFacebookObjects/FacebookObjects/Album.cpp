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
