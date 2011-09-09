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
 * User.h
 */

#ifndef USER_H_
#define USER_H_

#include "FacebookObject.h"

/**
 * Implements an User Facebook object type. It represents user profile.
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
