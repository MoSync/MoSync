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
