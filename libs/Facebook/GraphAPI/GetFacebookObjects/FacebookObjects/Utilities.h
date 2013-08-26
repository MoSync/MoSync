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
 * @file Utilities.h
 * @author Gabriela Rata
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <MAUtil/String.h>
#include <MAUtil/collection_common.h>

struct IdNamePair
{
	MAUtil::String mId;
	MAUtil::String mName;
};

struct CategoryData
{
	MAUtil::String mId;
	MAUtil::String mName;
	MAUtil::String mCategory;
	MAUtil::String mDateCreated;
};

struct Action
{
	MAUtil::String mName;
	MAUtil::String mLink;
};

struct Coordinate
{
	MAUtil::String mLatitude;
	MAUtil::String mLongitude;
};

struct EventResponse
{
	MAUtil::String mId;
	MAUtil::String mName;
	MAUtil::String mRsvp_Status;
};

enum EVENT_RESPONSE_TYPE { attending, maybe, declined };

struct Venue
{
	MAUtil::String mStreet;
	MAUtil::String mCity;
	MAUtil::String mState;
	MAUtil::String mZip;
	MAUtil::String mCountry;
	Coordinate     mCoordinate;

	void clear();
};

struct Place
{
	MAUtil::String	mId;
	MAUtil::String	mName;
	Venue			mLocation;
};

#endif /* UTILITIES_H_ */
