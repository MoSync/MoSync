/*
 * PhotoAlbum.h
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
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
	IdNamePair 			mFrom;
	MAUtil::String 		mName;
	MAUtil::String 		mDescription;
	MAUtil::String 		mLink;
	MAUtil::String 		mCoverPhotoId;
	int 				mCount;
};

#endif /* PHOTOALBUM_H_ */
