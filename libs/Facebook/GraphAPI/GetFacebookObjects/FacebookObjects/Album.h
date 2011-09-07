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
 * Album.h
 */

#ifndef PHOTOALBUM_H_
#define PHOTOALBUM_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * Implements an Album.  See: http://developers.facebook.com/docs/reference/api/album/
 */
class Album : public FacebookObject
{
public:

	Album();

	/**
	 * Setter/getter for the "from" field of an Album object.
	 * @param from - it represents the person/application that created the album. It contains an id and name.
	 */
	void setFromField(const IdNamePair &from);
	const IdNamePair &getFromField() const;

	/**
	 * Setter/getter for the "name" field of an Album object.
	 * @param name - the title of the album
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

	/**
	 * Setter/getter for the "description" field of an Album object.
	 * @param desc - the description of the album
	 */
	void setDescription(const MAUtil::String &desc);
	const MAUtil::String &getDescription() const;

	/**
	 * Setter/getter for the "link" field of an Album object.
	 * @param link - a link to this album on Facebook
	 */
	void setLink(const MAUtil::String &link);
	const MAUtil::String &getLink() const;

	/**
	 * Setter/getter for the "cover_photo" field of an Album object.
	 * @param id - the album cover photo ID.
	 */
	void setCoverPhotoId(const MAUtil::String &id);
	const MAUtil::String &getCoverPhotoId() const;

	/**
	 * Setter/getter for count.
	 * @param count - the number of photos in this album.
	 */
	void setCount(int count);
	int getCount() const;

private:
	IdNamePair			mFrom;
	MAUtil::String		mName;
	MAUtil::String		mDescription;
	MAUtil::String		mLink;
	MAUtil::String		mCoverPhotoId;
	int					mCount;
};

#endif /* PHOTOALBUM_H_ */
