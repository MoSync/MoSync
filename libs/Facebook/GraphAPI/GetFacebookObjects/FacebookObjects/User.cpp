/*
 * User.cpp
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */


#include "User.h"

/**
 * Setter/getter for the "first_name" field of an User object.
 * @param firstName - the user's first name
 */
void User::setFirstName(const MAUtil::String &firstName)
{
	mFirstName = firstName;
}
const MAUtil::String &User::getFirstName() const
{
	return mFirstName;
}

/**
 * Setter/getter for the "last_name" field of an User object.
 * @param lastName - the user's first name
 */
void User::setLastName(const MAUtil::String &lastName)
{
	mLastName = lastName;
}
const MAUtil::String &User::getLastName() const
{
	return mLastName;
}

/**
 * Setter/getter for the "name" field of an User object.
 * @param name - the user's full name
 */
void User::setName(const MAUtil::String &name)
{
	mName = name;
}
const MAUtil::String User::getName() const
{
	return mName;
}

/**
 * Setter/getter for the "link" field of an User object.
 * @param link - the URL of the profile for the user on Facebook
 */
void User::setLink(const MAUtil::String &link)
{
	mLink = link;
}
const MAUtil::String &User::getLink() const
{
	return mLink;
}

/**
 * Setter/getter for the "username" field of an User object.
 * @param username - the user's Facebook username
 */
void User::setUsername(const MAUtil::String &username)
{
	mUsername = username;
}
const MAUtil::String &User::getUsername() const
{
	return mUsername;
}

/**
 * Setter/getter for the "gender" field of an User object.
 * @param gender - the user's gender: female or male
 */
void User::setGender(const MAUtil::String &gender)
{
	mGender = gender;
}
const MAUtil::String &User::getGender() const
{
	return mGender;
}

/**
 * Setter/getter for the "locale" field of an User object.
 * @param locale - the user's locale
 */
void User::setLocale(const MAUtil::String &locale)
{
	mLocale = locale;
}
const MAUtil::String &User::getLocale() const
{
	return mLocale;
}
