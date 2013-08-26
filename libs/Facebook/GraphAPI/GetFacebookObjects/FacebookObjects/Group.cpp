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

/*
 * Group.cpp
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
