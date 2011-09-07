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
 * Video.h
 */

#ifndef VIDEO_H_
#define VIDEO_H_

#include <MAUtil/Vector.h>
#include "FacebookObject.h"
#include "Utilities.h"
#include "Comment.h"

/**
 * Implements an Video Facebook object type.
 * See: http://developers.facebook.com/docs/reference/api/video/
 */
class Video: public FacebookObject
{
public:
	/**
	 * Setter/getter for the "from" field of an Video object.
	 * @param from - it represents the person/application that created the album. It contains an id and name.
	 */
	void setFrom(const IdNamePair &from);
	const IdNamePair &getFrom() const;

	/**
	 * Setter/getter for the "tags" field of an Video object.
	 * @param tags - the users who are tagged in this video. Represents a vector of
	 * objects that contain an id and name.
	 */
	void setTags(MAUtil::Vector<IdNamePair> &tags);
	const MAUtil::Vector<IdNamePair> &getTags() const;

	/**
	 * Setter/getter for the "name" field of an Video object.
	 * @param name - the video title or caption
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

	/**
	 * Setter/getter for the "description" field of an Video object.
	 * @param desc - the description of the video
	 */
	void setDescription(const MAUtil::String &descr);
	const MAUtil::String &getDescription() const;

	/**
	 * Setter/getter for the "picture" field of an Video object.
	 * @param url - the URL for the thumbnail picture for the video
	 */
	void setPictureUrl(const MAUtil::String &url);
	const MAUtil::String &getPictureUrl()const;

	/**
	 * Setter/getter for the "source" field of an Video object.
	 * @param url - a URL to the raw, playable video file
	 */
	void setSource(const MAUtil::String &url);
	const MAUtil::String &getSource() const;

	/**
	 * Setter/getter for the "comments" field of an Video object.
	 * @param comments - the comments on this video
	 */
	void setComments(const MAUtil::Vector<Comment> &comments);
	const MAUtil::Vector<Comment> &getComments() const;

private:
	IdNamePair					mFrom;
	MAUtil::Vector<IdNamePair> 	mTags;
	MAUtil::String 				mName;
	MAUtil::String 				mDescription;
	MAUtil::String 				mPictureUrl;
	MAUtil::String 				mSource;
	MAUtil::Vector<Comment> 	mComments;
};

#endif /* VIDEO_H_ */
