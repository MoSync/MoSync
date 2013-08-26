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
 * @file Photo.h
 * @author Gabriela Rata
 */

#ifndef PHOTO_H_
#define PHOTO_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * \brief Implements an Photo Facebook object type.
 * See: http://developers.facebook.com/docs/reference/api/photo/
 */
class Photo: public FacebookObject
{
public:
	/**
	 * Constructor.
	 */
	Photo();

	/**
	 * Setter/getter for the "from" field of an Photo object.
	 * @param from - it represents the person/application that posted the photo.
	 * It contains an id and name.
	 */
	void setFrom(const IdNamePair &from);
	const IdNamePair &getFrom() const;

	/**
	 * Setter/getter for the "name" field of an Photo object.
	 * @param name - the caption of the photo
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

	/**
	 * Setter/getter for the "picture" field of an Photo object.
	 * @param url - represents a valid URL to the the thumbnail-sized photo.
	 */
	void setSmallSizedPicture(const MAUtil::String &url);
	const MAUtil::String &getSmallSizedPicture()const;

	/**
	 * Setter/getter for the "source" field of an Photo object.
	 * @param url - represents a valid URL to the the full-sized photo.
	 */
	void setFullSizedPicture(const MAUtil::String &url);
	const MAUtil::String &getFullSizedPicture()const;

	/**
	 * Setter/getter for the "width" field of an Photo object.
	 * @param width - The width of the photo in pixels.
	 */
	void setWidth(int width);
	int getWidth() const;

	/**
	 * Setter/getter for the "height" field of an Photo object.
	 * @param height - the height of the photo in pixels.
	 */
	void setHeight(int height);
	int getHeight() const;

	/**
	 * Setter/getter for the "link" field of an Photo object.
	 * @param link - a link to this photo on Facebook
	 */
	void setLink(const MAUtil::String &link);
	const MAUtil::String &getLink() const;

	/**
	 * Setter/getter for the "position" field of an Photo object.
	 * @param position - the position of this photo in the album.
	 */
	void setPositionInAlbum(int pos);
	int getPositionInAlbum() const;

private:
	IdNamePair		mFrom;
	MAUtil::String	mName;
	MAUtil::String	mSmallSizedPictureUrl;
	MAUtil::String	mFullSizedPictureUrl;
	int				mWidth;
	int				mHeight;
	MAUtil::String 	mLink;
	int				mPositionInAlbum;
};

#endif
