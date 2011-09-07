/*
 * Group.h
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#ifndef GROUP_H_
#define GROUP_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * Implements an Group Facebook object.
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
	IdNamePair 		mOwner;
	MAUtil::String  mName;
	MAUtil::String  mDescription;
	MAUtil::String  mLink;
	MAUtil::String  mPrivacy;
};

#endif /* GROUP_H_ */
