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
 * Event.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#include "Event.h"

void Event::setOwner(const IdNamePair &owner)
{
	mOwner = owner;
}
const IdNamePair &Event::getOwner() const
{
	return mOwner;
}

void Event::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Event::getName() const
{
	return mName;
}

void Event::setDescription(const MAUtil::String &descr)
{
	mDescription = descr;
}
const MAUtil::String &Event::getDescription() const
{
	return mDescription;
}

void Event::setStartTime(const MAUtil::String &startTime)
{
	mStartTime = startTime;
}
const MAUtil::String &Event::getStartTime() const
{
	return mStartTime;
}

void Event::setEndTime(const MAUtil::String &endTime)
{
	mEndTime = endTime;
}
const MAUtil::String &Event::getEndTime() const
{
	return mEndTime;
}

void Event::setLocation(const MAUtil::String &location)
{
	mLocation = location;
}
const MAUtil::String &Event::getLocation() const
{
	return mLocation;
}

void Event::setVenue(const Venue &venue)
{
	mVenue = venue;
}
const Venue &Event::getVenue() const
{
	return mVenue;
}

void Event::setPrivacy(const MAUtil::String &privacy)
{
	mPrivacy = privacy;
}
const MAUtil::String &Event::getPrivacy() const
{
	return mPrivacy;
}
