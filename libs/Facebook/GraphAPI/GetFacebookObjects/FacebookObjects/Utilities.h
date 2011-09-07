/*
 * Utilities.h
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
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
	MAUtil::String 	mId;
	MAUtil::String 	mName;
	Venue 			mLocation;
};

#endif /* UTILITIES_H_ */
