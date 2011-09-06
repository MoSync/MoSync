/*
 * FriendList.h
 *
 *  Created on: Jul 8, 2011
 *      Author: gabi
 */

#ifndef FRIENDLIST_H_
#define FRIENDLIST_H_

#include "Utilities.h"
#include "FacebookObject.h"

/**
 * Implements an FriendList Facebook object.
 * http://developers.facebook.com/docs/reference/api/FriendList/
 */
class FriendList: public FacebookObject
{
public:
	/**
	 * Setter/getter for the "name" field of an FriendList object.
	 * @param name - the friend list name
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

private:
	MAUtil::String mListName;
};

#endif /* FRIENDLIST_H_ */
