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
