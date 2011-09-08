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
 * Comment.cpp
 */

#include "Comment.h"

Comment::Comment()
{
	mLikes = -1;
}

/**
 * Setter/getter for the "message" field of an Comment object.
 * @param message - the comment text
 */
void Comment::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Comment::getMessage() const
{
	return mMessage;
}

/**
 * Setter/getter for the "from" field of an Comment object.
 * @param from - it represents the person/application that created the comment. It contains an id and name.
 */
void Comment::setFromField(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Comment::getFromField() const
{
	return mFrom;
}

/**
 * Setter/getter for the "likes" field of an Comment object.
 * @param likes - the number of times this comment was liked
 */
void Comment::setLikes(int likes)
{
	mLikes = likes;
}
int Comment::getLikes() const
{
	return mLikes;
}
