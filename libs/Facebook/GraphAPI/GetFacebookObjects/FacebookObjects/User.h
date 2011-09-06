/*
 * User.h
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
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
