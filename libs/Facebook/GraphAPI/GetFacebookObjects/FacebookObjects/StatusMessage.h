/* Copyright (C) 2011 MoSync AB

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
 * StatusMessage.h
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#ifndef STATUSMESSAGE_H_
#define STATUSMESSAGE_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * Implements an Post Facebook object type. It represents a status message on a user's wall.
 * See: http://developers.facebook.com/docs/reference/api/status/
 */
class StatusMessage: public FacebookObject
{
public:
	/**
	 * Setter/getter for the "from" field of an StatusMessage object.
	 * @param from - it represents the person/application that created the status message.
	 * It contains an id and name.
	 */
	void setFrom(const IdNamePair &from);
	const IdNamePair &getFrom() const;

	/**
	 * Setter/getter for the "message" field of an StatusMessage object.
	 * @param message - the status message content.
	 */
	void setMessage(const MAUtil::String &message);
	const MAUtil::String &getMessage() const;

private:
	IdNamePair mFrom;
	MAUtil::String mMessage;
};

#endif /* STATUSMESSAGE_H_ */
