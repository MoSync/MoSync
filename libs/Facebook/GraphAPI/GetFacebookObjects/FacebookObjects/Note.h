/*
 * Note.h
 *
 *  Created on: Jul 6, 2011
 *      Author: gabi
 */

#ifndef NOTE_H_
#define NOTE_H_

#include <MAUtil/Vector.h>
#include "FacebookObject.h"
#include "Comment.h"
#include "Utilities.h"

/**
 * Implements an Note. See: http://developers.facebook.com/docs/reference/api/note/
 */
class Note: public FacebookObject
{
public:
	/**
	 * Setter/getter for the "from" field of an Note object.
	 * @param from - it represents the user/application that created the note. It contains an id and name.
	 */
	void setFrom(const IdNamePair & from);
	const IdNamePair &getFrom() const;

	/**
	 * Setter/getter for the "subject" field of an Note object.
	 * @param subject - the title of the note
	 */
	void setSubject(const MAUtil::String &subject);
	const MAUtil::String &getSubject() const;

	/**
	 * Setter/getter for the "message" field of an Note object.
	 * @param message - the content of the note.
	 */
	void setMessage(const MAUtil::String &message);
	const MAUtil::String &getMessage() const;

	/**
	 * Setter/getter for the "comments" field of an Note object.
	 * @param comments - comments made on the note.
	 */
	void setComments(const MAUtil::Vector<Comment> &comments);
	const MAUtil::Vector<Comment> &getComments() const;

private:
	MAUtil::String 			mId;
	IdNamePair  			mFrom;
	MAUtil::String  		mSubject;
	MAUtil::String  		mMessage;
	MAUtil::Vector<Comment> mComments;
};


#endif /* NOTE_H_ */
