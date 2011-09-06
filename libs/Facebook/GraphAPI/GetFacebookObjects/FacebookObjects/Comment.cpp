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
