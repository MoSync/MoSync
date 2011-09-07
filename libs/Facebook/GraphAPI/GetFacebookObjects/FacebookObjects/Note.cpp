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
 * Note.cpp
 */


#include "Note.h"
#include "Comment.h"

/**
 * Setter/getter for the "from" field of an Note object.
 * @param from - it represents the user/application that created the note. It contains an id and name.
 */
void Note::setFrom(const IdNamePair & from)
{
	mFrom = from;
}
const IdNamePair &Note::getFrom() const
{
	return mFrom;
}

/**
 * Setter/getter for the "subject" field of an Note object.
 * @param subject - the title of the note
 */
void Note::setSubject(const MAUtil::String &subject)
{
	mSubject = subject;
}
const MAUtil::String &Note::getSubject() const
{
	return mSubject;
}

/**
 * Setter/getter for the "message" field of an Note object.
 * @param message - the content of the note.
 */
void Note::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &Note::getMessage() const
{
	return mMessage;
}

/**
 * Setter/getter for the "comments" field of an Note object.
 * @param comments - comments made on the note.
 */
void Note::setComments(const MAUtil::Vector<Comment> &comments)
{
	mComments.clear();
	mComments = comments;
}
const MAUtil::Vector<Comment> &Note::getComments() const
{
	return mComments;
}
