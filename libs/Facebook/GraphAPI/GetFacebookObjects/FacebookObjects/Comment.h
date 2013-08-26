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
 * @file Comment.h
 * @author Gabriela Rata
 */

#ifndef COMMENT_H_
#define COMMENT_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * \brief Implements an Comment Facebook object.
 * See: http://developers.facebook.com/docs/reference/api/Comment/
 */
class Comment: public FacebookObject
{

public:
	/**
	 * Constructor
	 */
	Comment();

	/**
	 * Setter/getter for the "message" field of an Comment object.
	 * @param message - the comment text
	 */
	void setMessage(const MAUtil::String &message);
	const MAUtil::String &getMessage() const;

	/**
	 * Setter/getter for the "from" field of an Comment object.
	 * @param from - it represents the person/application that created the comment. It contains an id and name.
	 */
	void setFromField(const IdNamePair &from);
	const IdNamePair &getFromField() const;

	/**
	 * Setter/getter for the "likes" field of an Comment object.
	 * @param likes - the number of times this comment was liked
	 */
	void setLikes(int likes);
	int getLikes() const;

private:
	MAUtil::String		mMessage;
	IdNamePair			mFrom;
	int					mLikes;
};

#endif /* COMMENT_H_ */
