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
 * @file Album.h
 * @author Gabriela Rata
 */

#ifndef PHOTOALBUM_H_
#define PHOTOALBUM_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * \brief Implements an Album object type.
 * See: http://developers.facebook.com/docs/reference/api/album/
 */
class Album : public FacebookObject
{
public:

	/**
	 * Constructor
	 */
	Album();

	/**
	 * Setter/getter for the "from" field of an Album object.
	 * @param from - it represents the person/application that created the
	 * album. It contains an id and name.
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
