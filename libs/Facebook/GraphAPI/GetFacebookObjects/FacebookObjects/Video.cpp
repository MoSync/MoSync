/*
 * Video.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#ifndef VIDEO_CPP_
#define VIDEO_CPP_

#include "Video.h"

/**
 * Setter/getter for the "from" field of an Video object.
 * @param from - it represents the person/application that created the album. It contains an id and name.
 */
void Video::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Video::getFrom() const
{
	return mFrom;
}

/**
 * Setter/getter for the "tags" field of an Video object.
 * @param tags - the users who are tagged in this video. Represents a vector of
 * objects that contain an id and name.
 */
void Video::setTags(MAUtil::Vector<IdNamePair> &tags)
{
	mTags.clear();
	for(int i=0; i<tags.size(); i++)
	{
		mTags.add(tags[i]);
	}
}
const MAUtil::Vector<IdNamePair> &Video::getTags() const
{
	return mTags;
}

/**
 * Setter/getter for the "name" field of an Video object.
 * @param name - the video title or caption
 */
void Video::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Video::getName() const
{
	return mName;
}

/**
 * Setter/getter for the "description" field of an Video object.
 * @param desc - the description of the video
 */
void Video::setDescription(const MAUtil::String &descr)
{
	mDescription = descr;
}
const MAUtil::String &Video::getDescription() const
{
	return mDescription;
}

/**
 * Setter/getter for the "picture" field of an Video object.
 * @param url - the URL for the thumbnail picture for the video
 */
void Video::setPictureUrl(const MAUtil::String &url)
{
	mPictureUrl = url;
}
const MAUtil::String &Video::getPictureUrl()const
{
	return mPictureUrl;
}

/**
 * Setter/getter for the "source" field of an Video object.
 * @param url - a URL to the raw, playable video file
 */
void Video::setSource(const MAUtil::String &url)
{
	mSource = url;
}
const MAUtil::String &Video::getSource() const
{
	return mSource;
}

/**
 * Setter/getter for the "comments" field of an Video object.
 * @param comments - the comments on this video
 */
void Video::setComments(const MAUtil::Vector<Comment> &comments)
{
	mComments.clear();
	for(int i=0; i<comments.size(); i++)
	{
		mComments.add(comments[i]);
	}
}
const MAUtil::Vector<Comment> &Video::getComments() const
{
	return mComments;
}

#endif /* VIDEO_CPP_ */
