/*
 * StatusMessage.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */


#include "StatusMessage.h"

/**
 * Setter/getter for the "from" field of an StatusMessage object.
 * @param from - it represents the person/application that created the status message.
 * It contains an id and name.
 */
void StatusMessage::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &StatusMessage::getFrom() const
{
	return mFrom;
}

/**
 * Setter/getter for the "message" field of an StatusMessage object.
 * @param message - the status message content.
 */
void StatusMessage::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &StatusMessage::getMessage() const
{
	return mMessage;
}
