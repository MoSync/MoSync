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
 * @file FacebookObject.h
 * @author Gabriela Rata
 */

#ifndef FACEBOOKOBJECT_H_
#define FACEBOOKOBJECT_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>

/**
 * \brief Base class for all the Facebook objects.
 * It contains the "id" field, that all the Facebook objects must have.
 */
class FacebookObject
{
protected:

	/**
	 * Constructor
	 * The constructor is made protected, to indicate that the purpose of this
	 * class is
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
