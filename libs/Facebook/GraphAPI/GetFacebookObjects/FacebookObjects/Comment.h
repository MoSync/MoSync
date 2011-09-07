/*
 * Comment.h
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#ifndef COMMENT_H_
#define COMMENT_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * Implements an Comment Facebook object.
 * See: http://developers.facebook.com/docs/reference/api/Comment/
 */
class Comment: public FacebookObject
{

public:
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
	MAUtil::String 		mMessage;
	IdNamePair			mFrom;
	int 				mLikes;
};

#endif /* COMMENT_H_ */
