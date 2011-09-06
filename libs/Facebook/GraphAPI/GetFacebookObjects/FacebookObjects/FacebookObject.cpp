/*
 * FacebookObject.cpp
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */

#include "FacebookObject.h"

/**
 * The constructor is made protected, to indicate that the purpose of this  class is
 * to be reused (by inheriting).
 */
FacebookObject::FacebookObject()
{
}

/**
 * Setter/getter for the "id" field of an Facebook object.
 * @param id - the id of the object.
 */
void FacebookObject::setId(const MAUtil::String &id)
{
	mId = id;
}

const MAUtil::String &FacebookObject::getId() const
{
	return mId;
}
