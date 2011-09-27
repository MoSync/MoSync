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
 * @file Link.h
 * @author Gabriela Rata
 */

#ifndef LINK_H_
#define LINK_H_

#include "FacebookObject.h"
#include "Comment.h"
#include <MAUtil/Vector.h>

/**
 * \brief Implements an Link Facebook object. Represents a link shared on a
 * user's wall.
 * See: http://developers.facebook.com/docs/reference/api/group/
 */
class Link: public FacebookObject
{
public:
	/**
	 * Setter/getter for the "from" field of an Link object.
	 * @param from - it represents the user that created the link. It contains
	 * an id and name.
	 */
	void setFrom(const IdNamePair &from);
	const IdNamePair &getFrom() const;

	/**
	 * Setter/getter for the "link" field of an Link object.
	 * @param link - the URL that was shared
	 */
	void setLink(const MAUtil::String &name);
	const MAUtil::String &getLink() const;

	/**
	 * Setter/getter for the "name" field of an Link object.
	 * @param name - the name of the link
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

	/**
	 * Setter/getter for the "comments" field of an Link object.
	 * @param comments - all of the comments on this link
	 */
	void setComments(MAUtil::Vector<Comment> &comments);
	const MAUtil::Vector<Comment> &getComments() const;

	/**
	 * Setter/getter for the "description" field of an Link object.
	 * @param descr - a description of the link (appears beneath the link caption).
	 */
	void setDescription(const MAUtil::String &descr);
	const MAUtil::String &getDescription() const;

	/**
	 * Setter/getter for the "picture" field of an Link object.
	 * @param pictureUrl - a URL to the thumbnail image used in the link post.
	 */
	void setPicture(const MAUtil::String &pictureUrl);
	const MAUtil::String &getPicture() const;

	/**
	 * Setter/getter for the "message" field of an Link object.
	 * @param message - the message from the user about this link.
	 */
	void setMessage(const MAUtil::String &message);
	const MAUtil::String &getMessage() const;

private:
	IdNamePair					mFrom;
	MAUtil::String				mLink;
	MAUtil::String				mName;
	MAUtil::Vector<Comment>		mComments;
	MAUtil::String				mDescription;
	MAUtil::String				mPicture;
	MAUtil::String				mMessage;
};

#endif /* LINK_H_ */
