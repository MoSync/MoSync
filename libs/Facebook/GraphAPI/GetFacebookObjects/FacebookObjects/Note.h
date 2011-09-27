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

/**
 * @file Note.h
 * @author Gabriela Rata
 */

#ifndef NOTE_H_
#define NOTE_H_

#include <MAUtil/Vector.h>
#include "FacebookObject.h"
#include "Comment.h"
#include "Utilities.h"

/**
 * \brief Implements an Note.
 * See: http://developers.facebook.com/docs/reference/api/note/
 */
class Note: public FacebookObject
{
public:

	/**
	 * Setter/getter for the "from" field of an Note object.
	 * @param from - it represents the user/application that created the note.
	 * It contains an id and name.
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
	MAUtil::String			mId;
	IdNamePair				mFrom;
	MAUtil::String			mSubject;
	MAUtil::String			mMessage;
	MAUtil::Vector<Comment> mComments;
};


#endif /* NOTE_H_ */
