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
 * Photo.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#include "Photo.h"

Photo::Photo()
{
	mWidth = -1;
	mHeight = -1;
	mPositionInAlbum = -1;
}

void Photo::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Photo::getFrom() const
{
	return mFrom;
}

void Photo::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Photo::getName() const
{
	return mName;
}

void Photo::setSmallSizedPicture(const MAUtil::String &url)
{
	mSmallSizedPictureUrl = url;
}
const MAUtil::String &Photo::getSmallSizedPicture()const
{
	return mSmallSizedPictureUrl;
}

void Photo::setFullSizedPicture(const MAUtil::String &url)
{
	mFullSizedPictureUrl = url;
}
const MAUtil::String &Photo::getFullSizedPicture()const
{
	return mFullSizedPictureUrl;
}

void Photo::setWidth(int width)
{
	mWidth = width;
}
int Photo::getWidth() const
{
	return mWidth;
}

void Photo::setHeight(int height)
{
	mHeight = height;
}
int Photo::getHeight() const
{
	return mHeight;
}

void Photo::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Photo::getLink() const
{
	return mLink;
}

void Photo::setPositionInAlbum(int pos)
{
	mPositionInAlbum = pos;
}
int Photo::getPositionInAlbum() const
{
	return mPositionInAlbum;
}
