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
 * Comment.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#include "Comment.h"

Comment::Comment()
{
	mLikes = -1;
}

void Comment::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Comment::getMessage() const
{
	return mMessage;
}

void Comment::setFromField(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Comment::getFromField() const
{
	return mFrom;
}

void Comment::setLikes(int likes)
{
	mLikes = likes;
}
int Comment::getLikes() const
{
	return mLikes;
}
