/*
 * FacebookObject.h
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */

#ifndef FACEBOOKOBJECT_H_
#define FACEBOOKOBJECT_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>

/**
 * Base class for all the Facebook objects.
 * It contains the "id" field, that all the Facebook objects must have.
 */
class FacebookObject
{
protected:
	/**
	 * The constructor is made protected, to indicate that the purpose of this  class is
	 * to be reused (by inheriting it).
	 */
	FacebookObject();

public:
	/**
	 * Setter/getter for the "id" field of an Facebook object.
	 * @param id - the id of the object.
	 */
	void setId(const MAUtil::String &id);
	const MAUtil::String &getId() const;

private:
	MAUtil::String mId;
};


#endif /* FACEBOOKOBJECT_H_ */
