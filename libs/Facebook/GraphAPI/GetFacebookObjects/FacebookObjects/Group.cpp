/*
 * Group.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#include "Group.h"

/**
 * Setter/getter for the "owner" field of an Group object.
 * @param message - the profile that created the group.
 */
void Group::setOwner(const IdNamePair &owner)
{
	mOwner = owner;
}
const IdNamePair &Group::getOwner() const
{
	return mOwner;
}

/**
 * Setter/getter for the "name" field of an Group object.
 * @param name - the name of the group.
 */
void Group::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Group::getName() const
{
	return mName;
}

/**
 * Setter/getter for the "description" field of an Group object.
 * @param descr - a description of the group.
 */
void Group::setDescription(const MAUtil::String &descr)
{
	mDescription = descr;
}
const MAUtil::String &Group::getDescription() const
{
	return mDescription;
}

/**
 * Setter/getter for the "link" field of an Group object.
 * @param link - the URL for the group's website.
 */
void Group::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Group::getLink() const
{
	return mLink;
}

/**
 * Setter/getter for the "privacy" field of an Group object.
 * @param privacy - the privacy setting of the group. It can have the following values:
 * OPEN, CLOSED, or SECRET
 */
void Group::setPrivacy(const MAUtil::String &privacy)
{
	mPrivacy = privacy;
}
const MAUtil::String &Group::getPrivacy() const
{
	return mPrivacy;
}
