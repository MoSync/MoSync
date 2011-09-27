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
