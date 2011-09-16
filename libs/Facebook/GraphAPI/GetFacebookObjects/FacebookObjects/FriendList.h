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

/**
 * @file FriendList.h
 * @author Gabriela Rata
 */

#ifndef FRIENDLIST_H_
#define FRIENDLIST_H_

#include "Utilities.h"
#include "FacebookObject.h"

/**
 * \brief Implements an FriendList Facebook object.
 * See: http://developers.facebook.com/docs/reference/api/FriendList/
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
