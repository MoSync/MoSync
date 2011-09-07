/*
 * FriendList.cpp
 *
 *  Created on: Jul 9, 2011
 *      Author: gabi
 */

#include "FriendList.h"

/**
 * Setter/getter for the "name" field of an FriendList object.
 * @param name - the friend list name
 */
void FriendList::setName(const MAUtil::String &name)
{
	mListName = name;
}
const MAUtil::String &FriendList::getName() const
{
	return mListName;
}
