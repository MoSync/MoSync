/*
 * Photo.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#include "Photo.h"

Photo::Photo()
{
	mWidth = -1;
	mHeight = -1;
	mPositionInAlbum = -1;
}

/**
 * Setter/getter for the "from" field of an Photo object.
 * @param from - it represents the person/application that posted the photo. It contains an id and name.
 */
void Photo::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &Photo::getFrom() const
{
	return mFrom;
}

/**
 * Setter/getter for the "name" field of an Photo object.
 * @param name - the caption of the photo
 */
void Photo::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Photo::getName() const
{
	return mName;
}

/**
 * Setter/getter for the "picture" field of an Photo object.
 * @param url - represents a valid URL to the the thumbnail-sized photo.
 */
void Photo::setSmallSizedPicture(const MAUtil::String &url)
{
	mSmallSizedPictureUrl = url;
}
const MAUtil::String &Photo::getSmallSizedPicture()const
{
	return mSmallSizedPictureUrl;
}

/**
 * Setter/getter for the "source" field of an Photo object.
 * @param url - represents a valid URL to the the full-sized photo.
 */
void Photo::setFullSizedPicture(const MAUtil::String &url)
{
	mFullSizedPictureUrl = url;
}
const MAUtil::String &Photo::getFullSizedPicture()const
{
	return mFullSizedPictureUrl;
}

/**
 * Setter/getter for the "width" field of an Photo object.
 * @param width - The width of the photo in pixels.
 */
void Photo::setWidth(int width)
{
	mWidth = width;
}
int Photo::getWidth() const
{
	return mWidth;
}

/**
 * Setter/getter for the "height" field of an Photo object.
 * @param height - the height of the photo in pixels.
 */
void Photo::setHeight(int height)
{
	mHeight = height;
}
int Photo::getHeight() const
{
	return mHeight;
}

/**
 * Setter/getter for the "link" field of an Photo object.
 * @param link - a link to this photo on Facebook
 */
void Photo::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Photo::getLink() const
{
	return mLink;
}

/**
 * Setter/getter for the "position" field of an Photo object.
 * @param position - the position of this photo in the album.
 */
void Photo::setPositionInAlbum(int pos)
{
	mPositionInAlbum = pos;
}
int Photo::getPositionInAlbum() const
{
	return mPositionInAlbum;
}
