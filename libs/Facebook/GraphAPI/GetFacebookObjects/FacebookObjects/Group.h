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
 * @file Group.h
 * @author Gabriela Rata
 */

#ifndef GROUP_H_
#define GROUP_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * \brief Implements an Group Facebook object.
 * See: http://developers.facebook.com/docs/reference/api/group/
 */
class Group: public FacebookObject
{
public:

	/**
	 * Setter/getter for the "owner" field of an Group object.
	 * @param message - the profile that created the group.
	 */
	void setOwner(const IdNamePair &owner);
	const IdNamePair &getOwner() const;

	/**
	 * Setter/getter for the "name" field of an Group object.
	 * @param name - the name of the group.
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

	/**
	 * Setter/getter for the "description" field of an Group object.
	 * @param descr - a description of the group.
	 */
	void setDescription(const MAUtil::String &descr);
	const MAUtil::String &getDescription() const;

	/**
	 * Setter/getter for the "link" field of an Group object.
	 * @param link - the URL for the group's website.
	 */
	void setLink(const MAUtil::String &link);
	const MAUtil::String &getLink() const;

	/**
	 * Setter/getter for the "privacy" field of an Group object.
	 * @param privacy - the privacy setting of the group. It can have the following values:
	 * OPEN, CLOSED, or SECRET
	 */
	void setPrivacy(const MAUtil::String &privacy);
	const MAUtil::String &getPrivacy() const;

private:
	IdNamePair		mOwner;
	MAUtil::String	mName;
	MAUtil::String	mDescription;
	MAUtil::String	mLink;
	MAUtil::String	mPrivacy;
};

#endif /* GROUP_H_ */
