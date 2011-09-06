/*
 * Utilities.cpp
 *
 *  Created on: Jul 2, 2011
 *      Author: gabi
 */

#include "Utilities.h"


void Venue::clear()
{
	mStreet.clear();
	mCity.clear();
	mState.clear();
	mZip.clear();
	mCountry.clear();
	mCoordinate.mLatitude.clear();
	mCoordinate.mLongitude.clear();
}
