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
 * Event.cpp
 */

#include "Event.h"

/**
 * Setter/getter for the "owner" field of an Event object.
 * @param message - the profile that created the event.
 */
void Event::setOwner(const IdNamePair &owner)
{
	mOwner = owner;
}
const IdNamePair &Event::getOwner() const
{
	return mOwner;
}

/**
 * Setter/getter for the "name" field of an Event object.
 * @param name - the event title
 */
void Event::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String &Event::getName() const
{
	return mName;
}

/**
 * Setter/getter for the "description" field of an Event object.
 * @param name - the description of the event.
 */
void Event::setDescription(const MAUtil::String &descr)
{
	mDescription = descr;
}
const MAUtil::String &Event::getDescription() const
{
	return mDescription;
}

/**
 * Setter/getter for the "start_time" field of an Event object.
 * @param startTime - the start time of the event
 */
void Event::setStartTime(const MAUtil::String &startTime)
{
	mStartTime = startTime;
}
const MAUtil::String &Event::getStartTime() const
{
	return mStartTime;
}

/**
 * Setter/getter for the "start_time" field of an Event object.
 * @param endTime - the end time of the event
 */
void Event::setEndTime(const MAUtil::String &endTime)
{
	mEndTime = endTime;
}
const MAUtil::String &Event::getEndTime() const
{
	return mEndTime;
}

/**
 * Setter/getter for the "location" field of an Event object.
 * @param location - the location for this event
 */
void Event::setLocation(const MAUtil::String &location)
{
	mLocation = location;
}
const MAUtil::String &Event::getLocation() const
{
	return mLocation;
}

/**
 * Setter/getter for the "venue" field of an Event object.
 * @param venue - the location for this event. A venue object contains:
 * street, city, state, zip, country, latitude, and longitude fields.
 */
void Event::setVenue(const Venue &venue)
{
	mVenue = venue;
}
const Venue &Event::getVenue() const
{
	return mVenue;
}

/**
 * Setter/getter for the "privacy" field of an Event object.
 * @param privacy - the visibility of this event. Can have the following values: "OPEN", "CLOSED", or "SECRET".
 */
void Event::setPrivacy(const MAUtil::String &privacy)
{
	mPrivacy = privacy;
}
const MAUtil::String &Event::getPrivacy() const
{
	return mPrivacy;
}
