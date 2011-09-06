/*
 * Note.cpp
 *
 *  Created on: Jul 6, 2011
 *      Author: gabi
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
