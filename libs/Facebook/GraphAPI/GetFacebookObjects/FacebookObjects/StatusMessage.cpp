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
 * StatusMessage.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */


#include "StatusMessage.h"

void StatusMessage::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &StatusMessage::getFrom() const
{
	return mFrom;
}

void StatusMessage::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &StatusMessage::getMessage() const
{
	return mMessage;
}
