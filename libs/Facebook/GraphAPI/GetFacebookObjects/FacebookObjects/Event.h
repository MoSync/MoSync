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
 * @file Event.h
 * @author Gabriela Rata
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * \brief Implements an Event Facebook object.
 * See: http://developers.facebook.com/docs/reference/api/event/
 */
class Event: public FacebookObject
{
public:

	/**
	 * Setter/getter for the "owner" field of an Event object.
	 * @param owner - the profile that created the event.
	 */
	void setOwner(const IdNamePair &owner);
	const IdNamePair &getOwner() const;

	/**
	 * Setter/getter for the "name" field of an Event object.
	 * @param name - the event title
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

	/**
	 * Setter/getter for the "description" field of an Event object.
	 * @param name - the description of the event.
	 */
	void setDescription(const MAUtil::String &descr);
	const MAUtil::String &getDescription() const;

	/**
	 * Setter/getter for the "start_time" field of an Event object.
	 * @param startTime - the start time of the event
	 */
	void setStartTime(const MAUtil::String &startTime);
	const MAUtil::String &getStartTime() const;

	/**
	 * Setter/getter for the "start_time" field of an Event object.
	 * @param endTime - the end time of the event
	 */
	void setEndTime(const MAUtil::String &endTime);
	const MAUtil::String &getEndTime() const;

	/**
	 * Setter/getter for the "location" field of an Event object.
	 * @param location - the location for this event
	 */
	void setLocation(const MAUtil::String &location);
	const MAUtil::String &getLocation() const;

	/**
	 * Setter/getter for the "venue" field of an Event object.
	 * @param venue - the location for this event. A venue object contains:
	 * street, city, state, zip, country, latitude, and longitude fields.
	 */
	void setVenue(const Venue &venue);
	const Venue &getVenue() const;

	/**
	 * Setter/getter for the "privacy" field of an Event object.
	 * @param privacy - the visibility of this event. Can have the following values: "OPEN", "CLOSED", or "SECRET".
	 */
	void setPrivacy(const MAUtil::String &privacy);
	const MAUtil::String &getPrivacy() const;

private:
	IdNamePair		mOwner;
	MAUtil::String	mName;
	MAUtil::String	mDescription;
	MAUtil::String	mStartTime;
	MAUtil::String	mEndTime;
	MAUtil::String	mLocation;
	Venue			mVenue;
	MAUtil::String	mPrivacy;
};

#endif /* EVENT_H_ */
