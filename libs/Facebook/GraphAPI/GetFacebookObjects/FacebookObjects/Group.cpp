/* Copyright (C) 2011 MoSync AB

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
 * Group.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#include "Group.h"

void Group::setOwner(const IdNamePair &owner)
{
	mOwner = owner;
}
const IdNamePair &Group::getOwner() const
{
	return mOwner;
}

void Group::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Group::getName() const
{
	return mName;
}

void Group::setDescription(const MAUtil::String &descr)
{
	mDescription = descr;
}
const MAUtil::String &Group::getDescription() const
{
	return mDescription;
}

void Group::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &Group::getLink() const
{
	return mLink;
}

void Group::setPrivacy(const MAUtil::String &privacy)
{
	mPrivacy = privacy;
}
const MAUtil::String &Group::getPrivacy() const
{
	return mPrivacy;
}
