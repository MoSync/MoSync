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
