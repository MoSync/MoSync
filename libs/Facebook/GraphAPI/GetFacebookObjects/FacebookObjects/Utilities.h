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

/*
 * Utilities.h
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
