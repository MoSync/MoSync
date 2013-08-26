/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file StatusMessage.h
 * @author Gabriela Rata
 */

#ifndef STATUSMESSAGE_H_
#define STATUSMESSAGE_H_

#include "FacebookObject.h"
#include "Utilities.h"

/**
 * \brief Implements an Post Facebook object type. It represents a status
 * message on a user's wall.
 * See: http://developers.facebook.com/docs/reference/api/status/
 */
class StatusMessage: public FacebookObject
{
public:
	/**
	 * Setter/getter for the "from" field of an StatusMessage object.
	 * @param from - it represents the person/application that created the
	 * status message.
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
