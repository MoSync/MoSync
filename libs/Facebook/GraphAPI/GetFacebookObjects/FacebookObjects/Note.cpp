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
 * Note.cpp
 *
 *  Created on: Jul 6, 2011
 *      Author: gabi
 */


#include "Note.h"
#include "Comment.h"

void Note::setFrom(const IdNamePair & from)
{
	mFrom = from;
}
const IdNamePair &Note::getFrom() const
{
	return mFrom;
}

void Note::setSubject(const MAUtil::String &subject)
{
	mSubject = subject;
}
const MAUtil::String &Note::getSubject() const
{
	return mSubject;
}

void Note::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Note::getMessage() const
{
	return mMessage;
}

void Note::setComments(const MAUtil::Vector<Comment> &comments)
{
	mComments.clear();
	mComments = comments;
}
const MAUtil::Vector<Comment> &Note::getComments() const
{
	return mComments;
}
