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
