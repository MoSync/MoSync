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
 * @file Video.h
 * @author Gabriela Rata
 */


#ifndef VIDEO_H_
#define VIDEO_H_

#include <MAUtil/Vector.h>
#include "FacebookObject.h"
#include "Utilities.h"
#include "Comment.h"

/**
 * \brief Implements an Video Facebook object type.
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
	MAUtil::Vector<IdNamePair>	mTags;
	MAUtil::String				mName;
	MAUtil::String				mDescription;
	MAUtil::String				mPictureUrl;
	MAUtil::String				mSource;
	MAUtil::Vector<Comment>		mComments;
};

#endif /* VIDEO_H_ */
