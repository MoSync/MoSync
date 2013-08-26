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
