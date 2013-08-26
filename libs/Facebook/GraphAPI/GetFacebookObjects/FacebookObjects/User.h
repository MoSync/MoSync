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
 * @file User.h
 * @author Gabriela Rata
 */

#ifndef USER_H_
#define USER_H_

#include "FacebookObject.h"

/**
 * \brief Implements an User Facebook object type. It represents a user profile.
 * See: http://developers.facebook.com/docs/reference/api/user/
 */
class User : public FacebookObject
{
public:

	/**
	 * Setter/getter for the "first_name" field of an User object.
	 * @param firstName - the user's first name
	 */
	void setFirstName(const MAUtil::String &firstName);
	const MAUtil::String &getFirstName() const;

	/**
	 * Setter/getter for the "last_name" field of an User object.
	 * @param lastName - the user's first name
	 */
	void setLastName(const MAUtil::String &lastName);
	const MAUtil::String &getLastName() const;

	/**
	 * Setter/getter for the "name" field of an User object.
	 * @param name - the user's full name
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String getName() const;

	/**
	 * Setter/getter for the "link" field of an User object.
	 * @param link - the URL of the profile for the user on Facebook
	 */
	void setLink(const MAUtil::String &link);
	const MAUtil::String &getLink() const;

	/**
	 * Setter/getter for the "username" field of an User object.
	 * @param username - the user's Facebook username
	 */
	void setUsername(const MAUtil::String &username);
	const MAUtil::String &getUsername() const;

	/**
	 * Setter/getter for the "gender" field of an User object.
	 * @param gender - the user's gender: female or male
	 */
	void setGender(const MAUtil::String &gender);
	const MAUtil::String &getGender() const;

	/**
	 * Setter/getter for the "locale" field of an User object.
	 * @param locale - the user's locale
	 */
	void setLocale(const MAUtil::String &locale);
	const MAUtil::String &getLocale() const;

private:
	MAUtil::String mFirstName;
	MAUtil::String mLastName;
	MAUtil::String mName;
	MAUtil::String mLink;
	MAUtil::String mUsername;
	MAUtil::String mGender;
	MAUtil::String mLocale;
};


#endif /* USER_H_ */
